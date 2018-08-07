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

#include "configstore.h"
#include "src/global.h"
#include "src/sql/nsqlquery.h"

#include <QVariant>

extern Global global;

//**********************
// Generic constructor.
//**********************
ConfigStore::ConfigStore(DatabaseConnection *conn)
{

    db = conn;
    db->lockForRead();
    // Check if the database exists.  If not, create it.
    NSqlQuery sql(db);
    sql.exec("Select * from sqlite_master where type='table' and name='ConfigStore';");
    if (!sql.next()) {
        db->unlock();
        sql.finish();
        this->createTable();
        return;
    }
    sql.finish();
    db->unlock();
}


//********************************************
// Create the table & set the initial values
//********************************************
void ConfigStore::createTable() {
    QLOG_TRACE() << "Entering ConfigStore::createTable()";

    QLOG_DEBUG() << "Creating table ConfigStore";
    NSqlQuery sql(db);

    db->lockForWrite();
    // build the SQL command & cretae the table
    QString command("Create table if not exists ConfigStore (" +
                  QString("key integer primary key not null unique,") +
                  QString("value blob)"));
    if (!sql.exec(command)) {
        QLOG_ERROR() << "Creation of ConfigStore table failed: " << sql.lastError();
        sql.finish();
        db->unlock();
    } else {
        // insert default values into the new table
        this->initTable();
        sql.finish();
        db->unlock();
    }
}



//*******************************************
// Insert default values into the new table
//*******************************************
void ConfigStore::initTable() {
    QLOG_TRACE() << "Entering ConfigStore::initTable()";

    QLOG_DEBUG() << "Initializing table ConfigStore";
    db->lockForWrite();
    NSqlQuery sql(db);
    sql.prepare("Insert into ConfigStore (key, value) values (:key, :value);");
    sql.bindValue(":key", CONFIG_STORE_LID);
    sql.bindValue(":value", 0);
    if (!sql.exec()) {
        QLOG_ERROR() << "Insertion of initial counter failed: " << sql.lastError();
    }
    sql.finish();
    db->unlock();
}


//*******************************************************************
// Every time we add a new object, we call this to get its unique
// local ID.  This number never changes
//*******************************************************************
qint32 ConfigStore::incrementLidCounter() {
    db->lockForWrite();

    NSqlQuery sql(db);
    // Prepare the SQL statement & fetch the row
    sql.prepare("Select value from ConfigStore where key=:key");
    sql.bindValue(":key", CONFIG_STORE_LID);
    if (!sql.exec()) {
        QLOG_ERROR() << "Fetch of ConfigStore LID counter statement failed: " << sql.lastError();
    }
    if (!sql.next()) {
        QLOG_ERROR() << "Fetch from ConfigStore failure: LID NOT FOUND!!!";
    } else {
        // Now that we have the next lid, increment the number & save it
        qint32 sequence = QVariant(sql.value(0)).toInt();
        sequence++;
        db->lockForWrite();
        sql.prepare("Update ConfigStore set value=:lid where key=:key;");
        sql.bindValue(":lid",sequence);
        sql.bindValue(":key", CONFIG_STORE_LID);
        if (!sql.exec()) {
            QLOG_ERROR() << "Error updating sequence number: " << sql.lastError();
        }
        sql.finish();
        db->unlock();

        // Return the next lid to the caller
        return sequence;
    }
    sql.finish();
    db->unlock();
    return -1;
}


//*******************************************************************
// Save a setting to the DB
//*******************************************************************
void ConfigStore::saveSetting(int key, QByteArray value) {
    NSqlQuery sql(db);
    // Prepare the SQL statement & fetch the row
    db->lockForWrite();
    sql.prepare("Delete from ConfigStore where key=:key");
    sql.bindValue(":key", key);
    sql.exec();

    sql.prepare("insert into ConfigStore (key, value) values (:key, :value)");
    sql.bindValue(":key", key);
    sql.bindValue(":value", value);
    sql.exec();
    sql.finish();
    db->unlock();
}


//*******************************************************************
// Return a value from the DB
//*******************************************************************
bool ConfigStore::getSetting(QByteArray &value, int key) {
    db->lockForRead();
    NSqlQuery sql(db);
    sql.prepare("select value from ConfigStore where key=:key");
    sql.bindValue(":key", key);
    sql.exec();
    while (sql.next()) {
        value = sql.value(0).toByteArray();
        sql.finish();
        db->unlock();
        return true;
    }
    sql.finish();
    db->unlock();
    return false;
}
