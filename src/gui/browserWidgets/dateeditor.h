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


#ifndef DATEEDITOR_H
#define DATEEDITOR_H

#include <QWidget>
#include <QDateTimeEdit>
#include "src/gui/browserWidgets/authoreditor.h"
#include "src/gui/browserWidgets/datetimeeditor.h"
#include "src/gui/browserWidgets/locationeditor.h"

#include "src/qevercloud/include/QEverCloud.h"
using namespace qevercloud;

using namespace std;


class DateEditor : public QWidget
{
    Q_OBJECT
private:


public:
    explicit DateEditor(QWidget *parent = 0);
    AuthorEditor authorEditor;
    LocationEditor locationEditor;
    DateTimeEditor subjectDate;
    DateTimeEditor createdDate;
    DateTimeEditor updatedDate;
    void setNote(qint32 lid, Note n);
    void setUpdateDate(qint64 value);
    void clear();
    
signals:
    void valueChanged();

protected:
    void hideEvent(QHideEvent* event);
    void showEvent(QShowEvent* event);
    
public slots:

private slots:
    void emitChangedSignal();
    
};

#endif // DATEEDITOR_H
