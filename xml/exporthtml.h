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



#ifndef EXPORTHTML_H
#define EXPORTHTML_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QtXml>
#include <QProgressDialog>

using namespace std;

#include "qevercloud/include/QEverCloud.h"
using namespace qevercloud;


class ExportHTML : public QObject
{
    Q_OBJECT

private:
    bool quitNow;
    bool writeIndex(QString directory);
    bool writeNotes(QString directory);
    void writeData(QString name, Data data);
    bool writeResource(QString directory, Note n, Resource r);
    QProgressDialog *progress;

public:
    explicit ExportHTML(QObject *parent = 0);
    void backupHTML(QString filename);
    int lastError;
    QString errorMessage;
    QList<qint32> lids;

signals:

public slots:
    void abortBackup();
};

#endif // EXPORTHTML_H
