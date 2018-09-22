/*********************************************************************************
NixNote - An open-source client for the Evernote service.
Copyright (C) 2013 Randy Baumgarte

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
***********************************************************************************/

#include "filewatcher.h"
#include "src/global.h"
#include "src/sql/configstore.h"
#include "src/sql/notetable.h"
#include "src/sql/notebooktable.h"
#include "src/sql/resourcetable.h"
#include "src/utilities/mimereference.h"
#include "src/sql/filewatchertable.h"
#include "src/xml/batchimport.h"
#include "src/sql/tagtable.h"

#include <QDirIterator>

#if QT_VERSION < 0x050000
#include <QtScript/QScriptEngine>
#else
#include <QJSEngine>
#endif



extern Global global;

FileWatcher::FileWatcher(QString dir, ScanType type, qint32 notebookLid, bool subdirs, QObject *parent) :
    QFileSystemWatcher(parent)
{
    QLOG_DEBUG() << "Setting up file watcher on directory " << dir << " subdirs=" << subdirs;

    this->notebookLid = notebookLid;
    this->dir = dir;
    this->scanType = type;
    this->includeSubdirectories = subdirs;
    addDirectory(dir);

    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(saveDirectory(QString)));
    connect(this, SIGNAL(fileChanged(QString)), this, SLOT(saveFile(QString)));
}


void FileWatcher::saveDirectory(QString dir){
    QStringList files;
    QStringList dirs;

    QLOG_DEBUG() << "Change in directory " << dir << " detected.. Proceed with file import";

    setupSubDirectories(dirs, files, dir);
    for (int i=0; i<files.size(); i++) {

        QString filename = files[i];
        if (filename.contains(".tmp", Qt::CaseInsensitive)) {
            QLOG_DEBUG() << "Ignoring file " << filename;
            continue;
        } else {
            QLOG_DEBUG() << "Importing file " << filename;
        }

        if (!saveFiles.contains(filename) || scanType == ImportDelete) {
            saveFiles.append(filename);
            saveFile(filename);
        }
    }
}

void FileWatcher::saveFile(QString filename) {
    QFileInfo fileInfo(filename);

    // If we have a dbi import file
    QLOG_TRACE() << fileInfo.dir().absolutePath() + QDir::separator();
    QLOG_TRACE() << global.fileManager.getDbiDirPath();

    if ((fileInfo.dir().absolutePath() + QDir::separator()) == global.fileManager.getDbiDirPath()) {
        BatchImport importer;
        importer.import(filename);
        emit(nnexImported());
        QFile f(filename);
        if (!f.remove()) {
            QLOG_ERROR() << "Error removing file: " << filename << ", err: " << f.errorString();
        }
        return;
    }


    // If we have a user-import file
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    f.close();
    if (f.size() == 0) {
        return;
    }

    Note newNote;
    NoteTable ntable(global.db);
    ConfigStore cs(global.db);
    qint32 lid = cs.incrementLidCounter();

    QCryptographicHash md5hash(QCryptographicHash::Md5);
    QByteArray hash = md5hash.hash(data, QCryptographicHash::Md5);

    // * Start setting up the new note
    newNote.guid = QString::number(lid);
    newNote.title = filename;

    NotebookTable bookTable(global.db);
    QString notebook;
    bookTable.getGuid(notebook, notebookLid);
    newNote.notebookGuid = notebook;
    newNote.active = true;
    newNote.created = QDateTime::currentMSecsSinceEpoch();;
    newNote.updated = newNote.created;
    newNote.updateSequenceNum = 0;
    NoteAttributes na;
// Windows Check
#ifndef _WIN32
    na.sourceURL = "file://" + filename;
#else
    na.sourceURL = "file:///"+file;
#endif  // end Windows check
    na.subjectDate = newNote.created;
    newNote.attributes = na;

    qint32 noteLid = lid;



    // BEGIN EXIT POINT
    QString exitName = "ExitPoint_ImportKeep";
    if (scanType == FileWatcher::ImportDelete) {
        exitName = "ExitPoint_ImportDelete";
    }

    QHash<QString, ExitPoint*> *points;
    points = global.exitManager->exitPoints;
    if (points->contains(exitName) &&
            points->value(exitName) != nullptr &&
            points->value(exitName)->getEnabled())
        exitPoint(points->value(exitName), newNote);
    // END EXIT POINT


    QString newNoteBody = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")+
           QString("<!DOCTYPE en-note SYSTEM \"http://xml.evernote.com/pub/enml2.dtd\">")+
           QString("<en-note style=\"word-wrap: break-word; -webkit-nbsp-mode: space; -webkit-line-break: after-white-space;\">");
    if (newNote.content.isSet())
        newNoteBody.append(newNote.content);

    MimeReference mimeRef;
    QString mime = mimeRef.getMimeFromFileName(filename);
    QString enMedia =QString("<en-media hash=\"") +hash.toHex() +QString("\" border=\"0\"")
            +QString(" type=\"" +mime +"\" ")
            +QString("/>");
    newNoteBody.append(enMedia + QString("</en-note>"));
    newNote.content = newNoteBody;
    ntable.add(lid, newNote, true);
    QString noteGuid = ntable.getGuid(lid);
    lid = cs.incrementLidCounter();


    // Start creating the new resource
    Resource newRes;
    Data d;
    d.body = data;
    d.bodyHash = hash;
    d.size = data.size();
    newRes.data = d;
    newRes.mime = mime;
    ResourceAttributes ra;
    ra.fileName = QFileInfo(f).fileName();
    if (mime.startsWith("image", Qt::CaseInsensitive) || mime.endsWith("pdf", Qt::CaseInsensitive))
        ra.attachment = false;
    else
        ra.attachment = true;
    newRes.active = true;
    newRes.guid = QString::number(lid);
    newRes.noteGuid = noteGuid;
    newRes.updateSequenceNum = 0;
    newRes.attributes = ra;
    ResourceTable restable(global.db);
    restable.add(lid, newRes, true, noteLid);

    emit(fileImported(noteLid, lid));

    if (scanType == FileWatcher::ImportDelete) {
        QLOG_DEBUG() << "Removing file " << filename;

        bool retval = f.remove();
        int count = 100;
        while (!retval && count > 0) {
            QLOG_ERROR() << "Error removing file: " << filename << ", err: " << f.errorString();
            count--;
        }
    }
}


void FileWatcher::setupSubDirectories(QStringList &directories, QStringList &files, QString root) {
    QDirIterator it(root, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString entry = it.next();
        QFileInfo fileInfo;
        fileInfo.setFile(entry);
        if (fileInfo.isFile() && !files.contains(fileInfo.absoluteFilePath())) {
            files.append(fileInfo.absoluteFilePath());
        }
        if (fileInfo.isDir() && !entry.endsWith("..") && !directories.contains(fileInfo.absoluteFilePath())) {
            directories.append(fileInfo.absoluteFilePath());
        }
    }
}



void FileWatcher::setupDirectory(QStringList &files, QString root) {
    QDirIterator it(root);
    files.clear();
    while (it.hasNext()) {
        QString entry = it.next();
        QFileInfo fileInfo;
        fileInfo.setFile(entry);
        if (fileInfo.isFile() && !files.contains(fileInfo.absoluteFilePath())) {
            files.append(fileInfo.absoluteFilePath());
        }
    }
}




void FileWatcher::addDirectory(QString root) {
    if (directories().contains(root))
        return;

    QStringList dirs;
    QStringList files;
    if (includeSubdirectories) {
        setupSubDirectories(dirs, files, root);
        if (!dirs.isEmpty())
           addPaths(dirs);
        if (!files.isEmpty())
            addPaths(files);
    } else {
        setupDirectory(files, root);
        if (!root.isEmpty())
            addPath(root);
        if (!files.isEmpty())
            addPaths(files);
    }
    if (scanType == ImportDelete) {
        for (int i=0; i<files.size(); i++) {
            saveFile(files[i]);
        }
    }
    if (scanType == ImportKeep)
        saveFiles = files;

}


void FileWatcher::exitPoint(ExitPoint *exit, Note &n) {
    QLOG_TRACE_IN();
    ExitPoint_FileImport *saveExit = new ExitPoint_FileImport();

#if QT_VERSION >= 0x050000
    QJSEngine engine;
    QJSValue exit_s = engine.newQObject(saveExit);
    engine.globalObject().setProperty("note", exit_s);
    // Start loading values
    QLOG_INFO() << tr("Calling exit ") << exit->getExitName();
    saveExit->setExitName(exit->getExitName());
    saveExit->setTitle(n.title);
    NotebookTable bookTable(global.db);
    Notebook book;
    bookTable.get(book, n.notebookGuid);
    if (!book.name.isSet())
        book.name = "unknown";
    saveExit->setNotebook(book.name);
    saveExit->setCreationDate(n.created);
    saveExit->setUpdatedDate(n.updated);
    saveExit->setSubjectDate(n.attributes->subjectDate);
//    saveExit->setTags(n.tagNames);
    saveExit->setContents("");
    saveExit->setFileName(n.attributes->sourceURL);

    // Set exit ready & call it.
    saveExit->setExitReady();
    QJSValue retval = engine.evaluate(exit->getScript());
    QLOG_INFO() << "Return value from exit: " << retval.toString();
#endif
#if QT_VERSION < 0x050000
    QScriptEngine scriptEngine;
    QScriptValue exit_qs = scriptEngine.newQObject(saveExit);
    scriptEngine.globalObject().setProperty("note", exit_qs);
    // Start loading values
    QLOG_INFO() << tr("Calling exit ") << exit->getExitName();

    // Set exit ready & call it.
    saveExit->setExitReady();
    QScriptValue retval = scriptEngine.evaluate(exit->getScript());
    QLOG_INFO() << "Return value from exit: " << retval.toString();
#endif

    // Check for any changes.
    if (saveExit->isTitleModified()) {
        n.title = saveExit->getTitle();
    }
    if (saveExit->isTagsModified()) {
        QStringList tagNames = saveExit->getTags();
        QStringList newTagNames;
        QStringList newTagGuids;
        TagTable ttable(global.db);
        for (int i=0; i<tagNames.size(); i++) {
            QString tagName = tagNames[i];
            QString tagGuid = "";
            qint32 tagLid;
            tagLid = ttable.findByName(tagName,0);
            if (tagLid > 0) {
                if (ttable.getGuid(tagGuid,tagLid)) {
                    newTagGuids.append(tagGuid);
                    newTagNames.append(tagName);
                } else
                    QLOG_ERROR() << tr("Tag was not found:") << tagName;
            } else
                QLOG_ERROR() << tr("Tag was not found:") << tagName;
        }
        n.tagGuids = newTagGuids;
        n.tagNames = newTagNames;
    }
    if (saveExit->isNotebookModified()) {
        NotebookTable ntable(global.db);
        QString notebookName = saveExit->getNotebook();
        qint32 notebookLid = ntable.findByName(notebookName);
        if (notebookLid >0) {
            QString notebookGuid = "";
            if (ntable.getGuid(notebookGuid, notebookLid))
                n.notebookGuid = notebookGuid;
            else
                QLOG_ERROR() << tr("Notebook was not found:") << notebookName;
        } else
            QLOG_ERROR() << tr("Notebook was not found:") << notebookName;
    }
    if (saveExit->isContentsModified()) {
        QByteArray data = saveExit->getContents().toUtf8();
        n.content = data;
    }

    QLOG_TRACE_OUT();
}
