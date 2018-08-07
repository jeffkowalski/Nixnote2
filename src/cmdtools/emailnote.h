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


#ifndef EMAILNOTE_H
#define EMAILNOTE_H

#include <QObject>
#include "src/email/smtpclient.h"
#include "src/email/mimemessage.h"

class EmailNote : public QObject
{
    Q_OBJECT
private:

public:
    explicit EmailNote(QObject *parent = 0);
    QString to;
    QString cc;
    QString bcc;
    QString comments;
    QString subject;
    QString note;
    bool ccSelf;
    int lastError;
    qint32 lid;
    QString errorMessage;
    QString contents;
    void unwrap(QString data);
    QString wrap();
    QStringList tokenizeString(QString value);
    void prepareEmailMessage(MimeMessage *message, QString note, QString body);
    QString stripContentsForPrint(QString contents);
    int sendEmail();

signals:

public slots:

};

#endif // EMAILNOTE_H
