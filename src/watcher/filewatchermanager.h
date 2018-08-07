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

#ifndef FILEWATCHERMANAGER_H
#define FILEWATCHERMANAGER_H

#include <QObject>
#include <QList>

#include "filewatcher.h"

class FileWatcherManager : public QObject
{
    Q_OBJECT
private:
    QList<FileWatcher*> importKeep;
    QList<FileWatcher*> importDelete;

public:
    explicit FileWatcherManager(QObject *parent = 0);
    void reset();
    void setup();
    void dump();
    
signals:
    void fileImported(qint32 noteLid, qint32 resourceLid);
    void fileImported();
    
public slots:
    void signalImported(qint32 noteLid, qint32 resourceLid);
    void signalImported();
    
};

#endif // FILEWATCHERMANAGER_H
