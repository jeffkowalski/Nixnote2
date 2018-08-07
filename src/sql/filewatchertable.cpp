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

#include "filewatchertable.h"
#include "src/sql/configstore.h"
#include "src/sql/nsqlquery.h"

FileWatcherTable::FileWatcherTable(DatabaseConnection *db)
{
    this->db = db;
}




// Add a new directory to watch
qint32 FileWatcherTable::addEntry(qint32 lid, QString baseDir, FileWatcher::ScanType type, qint32 notebookLid, bool includeSubdirs) {
    if (lid == 0) {
        ConfigStore cs(global.db);
        lid = cs.incrementLidCounter();
    }
    db->lockForWrite();
    NSqlQuery sql(db);
    sql.prepare("Insert Into DataStore (lid, key, data) values (:lid, :key, :data)");
    sql.bindValue(":lid", lid);
    sql.bindValue(":key", FILE_WATCHER_DIR);
    sql.bindValue(":data", baseDir);
    sql.exec();

    sql.bindValue(":lid", lid);
    sql.bindValue(":key", FILE_WATCHER_TYPE);
    sql.bindValue(":data", type);
    sql.exec();

    sql.bindValue(":lid", lid);
    sql.bindValue(":key", FILE_WATCHER_NOTEBOOK);
    sql.bindValue(":data", notebookLid);
    sql.exec();

    sql.bindValue(":lid", lid);
    sql.bindValue(":key", FILE_WATCHER_SUBDIRS);
    sql.bindValue(":data", includeSubdirs);
    sql.exec();
    sql.finish();
    db->unlock();
    return lid;
}



// Get an individual record
void FileWatcherTable::get(qint32 lid, QString &baseDir, FileWatcher::ScanType &type, qint32 &notebookLid, bool &includeSubdirs) {
    NSqlQuery sql(db);
    db->lockForRead();
    sql.prepare("Select key, data from DataStore where lid=:lid");
    sql.bindValue(":lid", lid);
    sql.exec();

    while(sql.next()) {
        int key = sql.value(0).toInt();
        switch(key) {
        case FILE_WATCHER_DIR :
            baseDir = sql.value(1).toString();
            break;
        case FILE_WATCHER_NOTEBOOK :
            notebookLid = sql.value(1).toInt();
            break;
        case FILE_WATCHER_SUBDIRS :
            includeSubdirs = sql.value(1).toBool();
            break;
        case FILE_WATCHER_TYPE : {
            int value = sql.value(1).toInt();
            if (value == FileWatcher::ImportDelete)
                type = FileWatcher::ImportDelete;
            else
                type = FileWatcher::ImportKeep;
         }
         default :
            break;
        }
    }
    db->unlock();
    sql.finish();
}




// Find the record by a directory name
qint32 FileWatcherTable::findLidByDir(QString baseDir) {
    NSqlQuery sql(db);
    db->lockForRead();
    sql.prepare("Select lid from DataStore where key=:key and data=:data");
    sql.bindValue(":key", FILE_WATCHER_DIR);
    sql.bindValue(":data", baseDir);
    sql.exec();
    qint32 retval = -1;
    if (sql.next())
       retval = sql.value(0).toInt();
    sql.finish();
    db->unlock();
    return retval;
}




// Get all LIDs for file watchers
qint32 FileWatcherTable::getAll(QList<qint32> &lids) {
    NSqlQuery sql(db);
    db->lockForRead();
    sql.prepare("Select lid from DataStore where key=:key");
    sql.bindValue(":key", FILE_WATCHER_DIR);
    sql.exec();
    lids.clear();
    while(sql.next())
        lids.append(sql.value(0).toInt());
    sql.finish();
    db->unlock();
    return lids.size();
}



// Remove a record
void FileWatcherTable::expunge(qint32 lid) {
    NSqlQuery sql(db);
    db->lockForWrite();
    sql.prepare("Delete from datastore where lid=:lid");
    sql.bindValue(":lid", lid);
    sql.exec();
    sql.finish();
    db->unlock();
}

