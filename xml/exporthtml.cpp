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


#include "xml/exporthtml.h"
#include "global.h"
#include "sql/notetable.h"
#include "sql/resourcetable.h"
#include "html/noteformatter.h"
#include <QProgressDialog>

extern Global global;


ExportHTML::ExportHTML(QObject *parent) :
    QObject(parent)
{
    lastError = 0;
    errorMessage = "";
    lids.empty();
}


void ExportHTML::backupHTML(QString directory) {
    quitNow = false;

    progress = new QProgressDialog();
    progress->setAutoClose(false);
    progress->setWindowModality(Qt::ApplicationModal);
    connect(progress, SIGNAL(canceled()), this, SLOT(abortBackup()));
    progress->setWindowTitle(tr("Export"));
    progress->show();

    if (writeIndex(directory))
        writeNotes(directory);

    progress->hide();
}


bool ExportHTML::writeIndex(QString directory) {
    QString filename = directory + "/Nixnote_index.html";
    QFile xmlFile(filename);
    if (!xmlFile.open(QIODevice::WriteOnly)) {
        lastError = 16;
        errorMessage = tr("Cannot open file.");
        return false;
    }

    QXmlStreamWriter * writer = new QXmlStreamWriter(&xmlFile);
    writer->setAutoFormatting(true);
    writer->setCodec("UTF-8");
    writer->writeStartDocument();
    writer->writeDTD("<!DOCTYPE html>");
    writer->writeStartElement("html");
    writer->writeStartElement("head");
    writer->writeTextElement("title", "Table of Contents");
    writer->writeStartElement("meta");
    writer->writeAttribute("http-equiv", "Content-Type");
    writer->writeAttribute("content", "text/html;charset=utf-8");
    writer->writeEndElement(); // meta
    writer->writeEndElement(); // head
    writer->writeStartElement("body");
    writer->writeTextElement("h1", "Table of Contents");
    writer->writeStartElement("ul");
    NoteTable table(global.db);
    for (int i=0; i<lids.size() && !quitNow; i++) {
        Note n;
        table.get(n, lids[i], true, true);
        if (n.title.isSet()) {
            writer->writeStartElement("li");
            writer->writeStartElement("a");
            writer->writeAttribute("href", "notes/" + n.guid + ".html");
            writer->writeCharacters(n.title);
            writer->writeEndElement(); // a
            writer->writeEndElement(); // li
        }
    }
    writer->writeEndElement(); // ul
    writer->writeEndElement(); // body
    writer->writeEndElement(); // html
    writer->writeEndDocument();
    xmlFile.close();

    return true;
}


static void writeRow(QXmlStreamWriter * writer, QString name, QString value) {
    writer->writeStartElement("tr");
    writer->writeStartElement("td");
    writer->writeStartElement("b");
    writer->writeCharacters(name);
    writer->writeEndElement(); // b
    writer->writeEndElement(); // td

    writer->writeStartElement("td");
    writer->writeStartElement("i");
    writer->writeCharacters(value);
    writer->writeEndElement(); // i
    writer->writeEndElement(); // td
    writer->writeEndElement(); // tr
}


bool ExportHTML::writeNotes(QString directory) {
    NoteTable table(global.db);
    QList<qint32> dirtyLids;
    table.getAllDirty(dirtyLids);
    progress->setMaximum(lids.size());
    progress->setLabelText(tr("Notes"));
    progress->setValue(0);
    QCoreApplication::processEvents();

    for (int i=0; i<lids.size() && !quitNow; i++) {
        progress->setValue(i+1);
        QCoreApplication::processEvents();
        Note n;
        table.get(n, lids[i], true, true);

        QDir().mkdir(directory + "/notes");
        QString filename = directory + "/notes/" + n.guid + ".html";
        QFile xmlFile(filename);
        if (!xmlFile.open(QIODevice::WriteOnly)) {
            lastError = 16;
            errorMessage = tr("Cannot open file.");
            return false;
        }
        QXmlStreamWriter * writer = new QXmlStreamWriter(&xmlFile);
        writer->setAutoFormatting(true);
        writer->setCodec("UTF-8");
        //writer->writeStartDocument();
        writer->writeDTD("<!DOCTYPE html>");
        writer->writeStartElement("html");
        writer->writeStartElement("head");
        writer->writeTextElement("title", n.title);
        writer->writeStartElement("meta");
        writer->writeAttribute("http-equiv", "Content-Type");
        writer->writeAttribute("content", "text/html;charset=utf-8");
        writer->writeEndElement(); // meta
        writer->writeStartElement("meta");
        writer->writeAttribute("name", "exporter-version");
        writer->writeAttribute("content", "nixnote");
        writer->writeEndElement(); // meta
        writer->writeStartElement("style");
        writer->writeAttribute("type", "css");
        writer->writeCharacters("body, td { font-family: Tahoma; font-size: 10pt; }");
        writer->writeEndElement(); // style
        writer->writeEndElement(); // head
        writer->writeStartElement("body");
        writer->writeTextElement("h1", n.title);
        writer->writeStartElement("div");
        writer->writeStartElement("table");
        writer->writeAttribute("bgcolor", "#D4DDE5");
        writer->writeAttribute("border", "0");
        {
            QDateTime timestamp;
            timestamp.setMSecsSinceEpoch(n.created);
            writeRow (writer, "Created:", timestamp.toString(Qt::SystemLocaleShortDate));
        }
        {
            QDateTime timestamp;
            timestamp.setMSecsSinceEpoch(n.updated);
            writeRow (writer, "Updated:", timestamp.toString(Qt::SystemLocaleShortDate));
        }
        {
            QStringList tags;
            for (int j=0; j<n.tagGuids.value().size(); j++)
                tags << n.tagNames.value()[j];
            tags.sort();
            writeRow(writer, "Tags:", tags.join(", "));
        }
        writer->writeEndElement(); // table
        writer->writeEndElement(); // div
        writer->writeStartElement("div");
        writer->writeCharacters("\n");
        {
            NoteFormatter noteFormatter(this);
            noteFormatter.setNote(n, true);
            QByteArray content = noteFormatter.rebuildNoteHTML();
            qint32 index = content.indexOf("<body");
            content.remove(0,index);
            index = content.indexOf(">");
            content.remove(0,index);
            index = content.indexOf("</body>");
            content.truncate(index);
            //       <a href="Gorilla Gutters_files/2015_11_23_17_59_14.pdf"><img src="Gorilla Gutters_files/16d382a5a5ed762a0bf7ab63fcf49909.png" alt="2015_11_23_17_59_14.pdf"></a></span>
            writer->device()->write(content);
        }
        writer->writeCharacters("\n");
        writer->writeEndElement(); // div
        writer->writeEndElement(); // body
        writer->writeEndElement(); // html
        //writer->writeEndDocument();

        xmlFile.close();

        QDir().mkdir(directory + "/notes/" + n.guid + "_files");
        if (n.resources.isSet())
            for (int j=0; j<n.resources.value().size(); j++)
                writeResource(directory, n, n.resources.value()[j]);
    }
    return true;
}


void ExportHTML::writeData(QString name, Data data) {
    (void)(name);
    (void)(data);
    // writer->writeStartElement(name);
    // if (data.body.isSet())
    //     createNode("Body", data.body.value());
    // if (data.bodyHash.isSet()) {
    //     createNode("BodyHash", data.bodyHash);
    //     QByteArray ba;
    //     ba.append(data.bodyHash);
    // }
    // if (data.size.isSet())
    //     createNode("Size", data.size);
    // writer->writeEndElement();
}


bool ExportHTML::writeResource(QString directory, Note note, Resource resource) {
    QString filename(directory + "/notes/" + note.guid + "_files/" + resource.guid);
    QFile resFile(filename);
    if (!resFile.open(QIODevice::WriteOnly)) {
        lastError = 16;
        errorMessage = tr("Cannot open file.");
        return false;
    }
    QDataStream writer(&resFile);
    Data data = resource.data;
    writer.writeBytes(data.body.value(), data.size);
    resFile.close();
    return true;
    // writer->writeStartElement("NoteResource");
    // if (r.guid.isSet())
    //     createNode("Guid", r.guid);
    // if (r.noteGuid.isSet())
    //     createNode("NoteGuid", r.noteGuid);
    // if (r.data.isSet())
    //     writeData("Data", r.data);
    // if (r.mime.isSet())
    //     createNode("Mime", r.mime);
    // if (r.width.isSet())
    //     createNode("Width", r.width);
    // if (r.height.isSet())
    //     createNode("Height",r.height);
    // if (r.duration.isSet())
    //     createNode("Duration", r.duration);
    // if (r.active.isSet())
    //     createNode("Active", r.active);
    // if (r.recognition.isSet())  {
    //     writeData("Recognition", r.recognition);
    // }
    // if (r.attributes.isSet()) {
    //     writer->writeStartElement("ResourceAttributes");
    //     if (r.attributes.value().sourceURL.isSet())
    //         createNode("SourceUrl", r.attributes.value().sourceURL);
    //     if (r.attributes.value().timestamp.isSet())
    //         createNode("Timestamp", QString::number(r.attributes.value().timestamp));
    //     if (r.attributes.value().latitude.isSet())
    //         createNode("Latitude", r.attributes.value().latitude);
    //     if (r.attributes.value().longitude.isSet())
    //         createNode("Longitude", r.attributes.value().longitude);
    //     if (r.attributes.value().altitude.isSet())
    //         createNode("Altitude", r.attributes.value().altitude);
    //     if (r.attributes.value().cameraMake.isSet())
    //         createNode("CameraMake", r.attributes.value().cameraMake);
    //     if (r.attributes.value().cameraModel.isSet())
    //         createNode("CameraModel", r.attributes.value().cameraModel);
    //     if (r.attributes.value().recoType.isSet())
    //         createNode("RecoType", r.attributes.value().recoType);
    //     if (r.attributes.value().fileName.isSet())
    //         createNode("FileName", r.attributes.value().fileName);
    //     if (r.attributes.value().attachment.isSet())
    //         createNode("Attachment", r.attributes.value().attachment);
    //     writer->writeEndElement();
    // }
    // if (r.updateSequenceNum.isSet())
    //     createNode("UpdateSequenceNumber", r.updateSequenceNum);
    // if (r.alternateData.isSet())
    //     writeData("AlternateData", r.alternateData);
    // writer->writeEndElement();
}


void ExportHTML::abortBackup() {
    quitNow = true;
    this->errorMessage = tr("Backup Canceled");
    this->lastError = 1;
    progress->close();
}
