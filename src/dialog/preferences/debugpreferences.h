#ifndef DEBUGPREFERENCES_H
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

#define DEBUGPREFERENCES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>


class DebugPreferences : public QWidget
{
    Q_OBJECT
private:
    QGridLayout *mainLayout;
    QCheckBox *disableUploads;
    QCheckBox *showLidColumn;
    QCheckBox *nonAsciiSortBug;
    QCheckBox *disableImageHighlight;
    QCheckBox *forceUTF8;
    QCheckBox *interceptSigHup;
    QCheckBox *multiThreadSave;
    QSpinBox *autoSaveInterval;

public:
    explicit DebugPreferences(QWidget *parent = 0);
    ~DebugPreferences();
    QString getDebugLevel();
    void saveValues();
    
signals:
    
public slots:
    
};

#endif // DEBUGPREFERENCES_H
