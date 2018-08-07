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

#ifndef RESOURCETABLE_H
#define RESOURCETABLE_H

#include "src/sql/nsqlquery.h"
#include "src/global.h"

#include <iostream>
#include <string>
#include <stdio.h>

#include <QSqlTableModel>
#include <QtSql>
#include <QString>

#define RESOURCE_GUID                    6000
#define RESOURCE_NOTE_LID                6001
#define RESOURCE_DATA_BODY               6002
#define RESOURCE_DATA_HASH               6003
#define RESOURCE_DATA_SIZE               6004
#define RESOURCE_MIME                    6005
#define RESOURCE_ACTIVE                  6006
#define RESOURCE_HEIGHT                  6007
#define RESOURCE_WIDTH                   6008
#define RESOURCE_DURATION                6009
#define RESOURCE_RECOGNITION_BODY        6010
#define RESOURCE_RECOGNITION_SIZE        6011
#define RESOURCE_RECOGNITION_HASH        6012
#define RESOURCE_UPDATE_SEQUENCE_NUMBER  6013
#define RESOURCE_ALTERNATE_BODY          6014
#define RESOURCE_ALTERNATE_SIZE          6015
#define RESOURCE_ALTERNATE_HASH          6016
#define RESOURCE_SOURCE_URL              6017
#define RESOURCE_CAMERA_MAKE             6018
#define RESOURCE_CAMERA_MODEL            6019
#define RESOURCE_ALTITUDE                6020
#define RESOURCE_LONGITUDE               6021
#define RESOURCE_LATITUDE                6022
#define RESOURCE_RECO_TYPE               6023
#define RESOURCE_ATTACHMENT              6024
#define RESOURCE_FILENAME                6025
#define RESOURCE_CLIENT_WILL_INDEX       6026
#define RESOURCE_ISDIRTY                 6027
#define RESOURCE_TIMESTAMP               6028
#define RESOURCE_INKNOTE                 6029

#define RESOURCE_INDEX_NEEDED            6999

using namespace std;


class ResourceTable
{

private:
    DatabaseConnection *db;
public:
    ResourceTable(DatabaseConnection *db);                             // Constructor

    // DB Read Functions
    qint32 getLid(QString noteGuid, QString guid);               // given a note & resource guid, return the lid
    qint32 getLid(string noteGuid, string guid);                 // Given a note & resource guid, return the lid
    qint32 getLid(string resourceGuid);                          // Given a GUID, return the lid
    qint32 getLid(QString resourceGuid);                         // Given a resource GUID, return the lid
    QString getGuid(int lid);                                    // Given a lid, get the guid
    bool get(Resource &resource, qint32 lid, bool withBinary);           // Get a resource given a lid
    bool get(Resource &resource, QString noteGuid, QString guid, bool withBinary);      // get a resource given a guid
    bool get(Resource &resource, string noteGuid, string guid, bool withBinary);        // get a resource given a guid
    bool isDirty(qint32 lid);                                    // Check if a resource is dirty
    bool isDirty(QString noteGuid, QString guid);                // Check if a resource is dirty
    bool isDirty(string noteGuid, string guid);                  // Check if a resource is dirty
    bool exists(qint32 lid);                                     // Does this resource exist?
    bool exists(QString noteGuid, QString guid);                 // Does this resource exist?
    bool exists(string noteGuid, string guid);                   // Does this resource exist?
    bool getResourceRecognition(Resource &resource, qint32 lid); // Get a resource's recognition data
    qint32 getLidByHashHex(QString noteGuid, QString hash);      // Get a lid by the resource's hash value
    bool getInkNote(QByteArray &value, qint32 lid);              // Get an inknote
    qint32 getIndexNeeded(QList<qint32> &lids);                  // Get a list of all resources needing indexing
    bool getResourceList(QList<qint32> &resourceList, qint32 noteLid);  // Get resources for a note
    qint32 getCount();                                           // count of all resources
    qint32 getUnindexedCount();                                  // count of unindexed resources
    qint32 getNoteLid(qint32 resLid);                            // Get the owning note for this resource
    QByteArray getDataHash(qint32 lid);                          // Get the hash value for the data in a resource
    void getResourceMap(QHash<QString, qint32> &map, QHash<qint32, Resource> &resourceMap, qint32 noteLid);  // Get a resource MAP data
    void getResourceMap(QHash<QString, qint32> &map, QHash<qint32, Resource> &resourceMap, string guid);     // Get a resource's MAP data
    void getResourceMap(QHash<QString, qint32> &map, QHash<qint32, Resource> &resourceMap, QString guid);    // Get a resource's MAP data
    void getAllResources(QList<Resource> &list, qint32 noteLid, bool fullLoad, bool withBinary);  // Get all resources for a note

    // DB Write Functions
    void updateGuid(qint32 lid, Guid &guid);                     // Update a resource's guid
    void sync(Resource &resource);                               // Sync a resource with a new record
    void sync(qint32 lid, Resource &resource);                   // Sync a resource with a new record
    qint32 add(qint32 lid, Resource &t, bool isDirty, int noteLid=0);    // Add a new resource
    void setIndexNeeded(qint32 lid, bool indexNeeded);           // flag if a resource needs reindexing
    void expunge(int lid);                                       // erase a resource
    void expunge(QString guid);                                  // erase a resource
    void updateResourceHash(qint32 lid, QByteArray newhash);     // Update a resource's hash value
    qint32 addStub(qint32 resLid, qint32 noteLid);               // Add a basic "stub" record.  Useful when duplicating notes
    void reindexAllResources();                                  // Reindex all relources
    void updateNoteLid(qint32 resourceLid, qint32 newNoteLid);   // Update the owning note
    void expungeByNote(qint32 notebookLid);                      // Given a note's LID, erase the resource
    void mapResource(NSqlQuery &query, Resource &resource);      // Save a resource map data
};


#endif // RESOURCETABLE_H
