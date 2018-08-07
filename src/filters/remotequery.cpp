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

#include "remotequery.h"
#include "src/global.h"
#include "filterengine.h"


extern Global global;

RemoteQuery::RemoteQuery(QObject *parent) :
    QObject(parent)
{
    this->initDbus();
    note = nullptr;
    lid = -1;
}



void RemoteQuery::initDbus() {
    return;
    QLOG_DEBUG() << "inside InitDbus()";
    if (!QDBusConnection::sessionBus().isConnected()) {
        return;
    }
    QLOG_DEBUG() << "registerintg service()";
    if (!QDBusConnection::sessionBus().registerService(DBUS_SERVICE_NAME)) {
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }
    QLOG_DEBUG() << "Registering object";
    QDBusConnection::sessionBus().registerObject("/com/canonical/unity/scope/notes/NixNote/RemoteQuery", this, QDBusConnection::ExportAllSlots);
    QLOG_DEBUG() << "leavining init";
}


Q_SCRIPTABLE bool RemoteQuery::setNote(qint32 lid) {
    this->lid = lid;
    NoteTable ntable(global.db);
    if (note != nullptr)
        delete note;
    note = new Note();
    if (ntable.get(*note, lid, false,false))
        return true;
    delete note;
    note = nullptr;
    return false;
}


Q_SCRIPTABLE QString RemoteQuery::getNoteTitle() {
    if (note == nullptr)
        return "";
    if (note->title.isSet())
        return note->title;
    return "";
}



Q_SCRIPTABLE QString RemoteQuery::query(const QString &arg) {
    QList<qint32> results;
    FilterCriteria filter;
    filter.setSearchString(arg);
    FilterEngine engine;
    engine.filter(&filter, &results);
    QString retVal = "";
    for (int i=0; i<results.size(); i++) {
        retVal = retVal+ QString::number(results[i]) + QString(" ");
    }
    return retVal.trimmed();
}


Q_SCRIPTABLE QString RemoteQuery::getNotePreview() {
    QString filename = global.fileManager.getThumbnailDirPath("")+QString::number(lid)+".png";
    QFile file(filename);
    if (file.exists()) {
        return filename;
    }
    return global.fileManager.getImageDirPath("")+QString("windowIcon0.png");

}

Q_SCRIPTABLE QString RemoteQuery::getNoteDateUpdated() {
    qlonglong dt = 0;
    if (note->updated.isSet())
        dt = note->updated;
    if (dt==0)
        return "";
    QDateTime timestamp;
    timestamp.setTime_t(dt/1000);

    if (timestamp.date() == QDate::currentDate())
        return tr("Today") +" " + timestamp.time().toString(Qt::SystemLocaleShortDate);
    return timestamp.toString(global.getDateTimeFormat());

}



Q_SCRIPTABLE QString RemoteQuery::getNoteDateCreated() {
    qlonglong dt = 0;
    if (note->created.isSet())
        dt = note->created;
    if (dt==0)
        return "";
    QDateTime timestamp;
    timestamp.setTime_t(dt/1000);

    if (timestamp.date() == QDate::currentDate())
        return tr("Today") +" " + timestamp.time().toString(Qt::SystemLocaleShortDate);
    return timestamp.toString(global.getDateTimeFormat());

}




Q_SCRIPTABLE QString RemoteQuery::getNoteTags() {
    if (!note->tagNames.isSet())
        return "";
    QString taglist = "";
    QList <QString> tagNames = note->tagNames;
    for (int i=0; i<tagNames.size(); i++) {
        if (taglist.size()>0)
            taglist = taglist +", ";
        taglist = taglist+tagNames[i];
    }
    return taglist;
}

