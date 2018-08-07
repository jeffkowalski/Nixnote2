/*********************************************************************************
NixNote - An open-source client for the Evernote service.
Copyright (C) 2014 Randy Baumgarte

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


//* Favorites Records are used to store information that the user
//* defines in the upper left corner as a "favorite".  It can be
//* many different types of items.

#ifndef FAVORITESRECORD_H
#define FAVORITESRECORD_H

#include <QObject>
#include <QVariant>

class FavoritesRecord;

class FavoritesRecord : public QObject
{
    Q_OBJECT
public:
    explicit FavoritesRecord(QObject *parent = 0);

    // What type of favorite record is it
    enum FavoritesRecordType {
        Note=1,
        Tag=2,
        Search=3,
        LocalNotebook=4,
        SynchronizedNotebook=5,
        ConflictNotebook=6,
        NotebookStack=7,
        SharedNotebook=8,
        LinkedNotebook=9,
        LinkedStack=10
    };


    qint32  lid;                // LID of the actual note/notebook/tag/search...
    QString displayName;        // Display nme to show in the favorites list
    qint32 order;               // Display order (lower ones show up first)
    FavoritesRecordType  type;  // The actual type of record we point to
    QVariant  target;           // The target of the record
    qint32 parent;              // The parent of this record


signals:

public slots:

};

#endif // FAVORITESRECORD_H
