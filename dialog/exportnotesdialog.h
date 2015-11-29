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


#ifndef EXPORTNOTESDIALOG_H
#define EXPORTNOTESDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include "gui/nmainmenubar.h"


class ExportNotesDialog : public QDialog
{
    Q_OBJECT

private:
    NMainMenuBar *menuBar;
    QWidget *parent;

    QWidget *verticalLayoutWidget;
    QVBoxLayout *mainVerticalLayout;
    QGroupBox *actionGroupBox;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *actionVBoxLayout;
    QRadioButton *nnexActionRadioButton;
    QRadioButton *htmlActionRadioButton;
    QGroupBox *descriptionGroupBox;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *descriptionVBoxLayout;
    QPlainTextEdit *descriptionPlainTextEdit;
    QHBoxLayout *buttonsHorizontalLayout;
    QPushButton *optionsPushButton;
    QPushButton *exportPushButton;
    QPushButton *closePushButton;

public:
    explicit ExportNotesDialog(NMainMenuBar *menuBar, QWidget *parent = 0);

signals:

public slots:
    void exportNotes();
    void actionRadioButtonToggled();
};

#endif // EXPORTNOTESDIALOG_H
