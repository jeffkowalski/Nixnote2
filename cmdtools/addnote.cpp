/*********************************************************************************
NixNote - An open-source client for the Evernote service.
Copyright (C) 2015 Randy Baumgarte

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


#include "addnote.h"
#include <QtXml>
#include <QDebug>
#include "global.h"
#include "sql/notetable.h"
#include "sql/resourcetable.h"

extern Global global;

AddNote::AddNote(QObject *parent) :
    QObject(parent)
{
    title = tr("Untitled Note");
    created = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzzZ");
    updated = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzzZ");
    reminder = "";
    attachmentDelimiter = "%%";
    lid = 0;
}



//*******************************************
//* Write out the file to the dbi directory.
//*******************************************
void AddNote::write(QString uuid) {

    QString filename = uuid +".nnex";

    // We use a temporary file to write to.  At the end it will be renamed into
    // the DBI directory.  We don't write into it because there can be
    // timing issues where the FileWatcher picks up the file before
    // the entire text is written out and it causes an error.
    QFile xmlFile(global.fileManager.getTmpDirPath()+filename);


    if (!xmlFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Unable to open file.";
        return;
    }

    QXmlStreamWriter *writer = new QXmlStreamWriter(&xmlFile);
    writer->setAutoFormatting(true);
    writer->setCodec("UTF-8");
    writer->writeStartDocument();
    writer->writeDTD("<!DOCTYPE NixNote-Import>");
    writer->writeStartElement("nixnote-import");
    writer->writeAttribute("version", "2");
    writer->writeAttribute("application", APP_NNEX_APP_NAME);
    writer->writeAttribute("applicationVersion", "2.x");
    writer->writeStartElement("NoteAdd");


    writer->writeTextElement("Lid", QString::number(lid));
    writer->writeTextElement("Title", title);

    writer->writeStartElement("Content");
    writer->writeCDATA(content);
    writer->writeEndElement();

    if (notebook != "")
        writer->writeTextElement("Notebook", notebook);
    if (created != "")
        writer->writeTextElement("Created", created);
    if (updated != "")
        writer->writeTextElement("Updated", updated);
    if (reminder != "")
        writer->writeTextElement("Reminder", reminder);
    for (int i=0; i<tags.size(); i++) {
        writer->writeTextElement("Tag", tags[i]);
    }
    for (int i=0; i<attachments.size(); i++) {
        writer->writeTextElement("Attachment", attachments[i]);
    }
    writer->writeTextElement("AttachmentDelimiter", attachmentDelimiter);
    writer->writeEndElement();
    writer->writeEndElement();
    writer->writeEndDocument();
    xmlFile.close();
    QFile::rename(global.fileManager.getTmpDirPath()+filename,global.fileManager.getDbiDirPath()+filename);
}



// Create  a new resource and add it to the database
qint32 AddNote::createResource(Resource &r, int sequence, QByteArray data,  QString mime, bool attachment, QString filename, qint32 noteLid) {
    qint32 lid = noteLid;
    ConfigStore cs(global.db);
    qint32 rlid = cs.incrementLidCounter();

    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);

    QString guid =  QString::number(rlid);
    NoteTable noteTable(global.db);
    r.guid = guid;
    r.noteGuid = noteTable.getGuid(lid);
    QString noteguid = r.noteGuid;
    if (noteguid == "")
        return 0;
    r.mime = mime;
    r.active = true;
    r.updateSequenceNum = sequence;
    r.width = 0;
    r.height = 0;
    r.duration = 0;
    ResourceAttributes a;
    if (r.attributes.isSet())
        a = r.attributes;
    a.attachment = attachment;
    if (filename != "") {
        a.fileName = filename;
    }

    Data d;
    d.body = data;
    d.bodyHash = hash;
    d.size = data.size();

    r.data = d;
    r.attributes = a;
    ResourceTable resourceTable(global.db);
    resourceTable.add(rlid, r, true, lid);

    return rlid;
}
