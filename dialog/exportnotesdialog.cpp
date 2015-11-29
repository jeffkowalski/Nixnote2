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

#include "exportnotesdialog.h"

extern Global global;

ExportNotesDialog::ExportNotesDialog(NMainMenuBar *menubar, QWidget *parent) :
    QDialog(parent)
{
    this->parent = parent;
    this->menuBar = menubar;

    verticalLayoutWidget = new QWidget(this);

    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(QRect(20, 10, 300, 269));
    mainVerticalLayout = new QVBoxLayout(verticalLayoutWidget);
    mainVerticalLayout->setSpacing(6);
    mainVerticalLayout->setContentsMargins(11, 11, 11, 11);
    mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
    mainVerticalLayout->setContentsMargins(0, 0, 0, 0);
    actionGroupBox = new QGroupBox(verticalLayoutWidget);
    actionGroupBox->setObjectName(QString::fromUtf8("actionGroupBox"));
    actionGroupBox->setTitle(tr("Choose an action to perform:"));
    verticalLayout_4 = new QVBoxLayout(actionGroupBox);
    verticalLayout_4->setSpacing(6);
    verticalLayout_4->setContentsMargins(11, 11, 11, 11);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

    actionVBoxLayout = new QVBoxLayout();
    actionVBoxLayout->setSpacing(6);
    actionVBoxLayout->setObjectName(QString::fromUtf8("actionVBoxLayout"));

    nnexActionRadioButton = new QRadioButton(actionGroupBox);
    nnexActionRadioButton->setObjectName(QString::fromUtf8("nnexActionRadioButton"));
    nnexActionRadioButton->setText(tr("Export as file in NixNote format (.&nnex)"));
    actionVBoxLayout->addWidget(nnexActionRadioButton);

    htmlActionRadioButton = new QRadioButton(actionGroupBox);
    htmlActionRadioButton->setObjectName(QString::fromUtf8("htmlActionRadioButton"));
    htmlActionRadioButton->setText(tr("Export as multiple web pages (.&html)"));
    htmlActionRadioButton->setChecked(true);
    actionVBoxLayout->addWidget(htmlActionRadioButton);

    actionVBoxLayout->setStretch(0, 1);
    actionVBoxLayout->setStretch(1, 1);

    verticalLayout_4->addLayout(actionVBoxLayout);

    mainVerticalLayout->addWidget(actionGroupBox);

    descriptionGroupBox = new QGroupBox(verticalLayoutWidget);
    descriptionGroupBox->setObjectName(QString::fromUtf8("descriptionGroupBox"));
    descriptionGroupBox->setTitle(tr("Description"));
    descriptionGroupBox->setMinimumSize(QSize(0, 100));
    descriptionGroupBox->setFlat(false);
    descriptionGroupBox->setCheckable(false);
    verticalLayout_5 = new QVBoxLayout(descriptionGroupBox);
    verticalLayout_5->setSpacing(6);
    verticalLayout_5->setContentsMargins(11, 11, 11, 11);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    descriptionVBoxLayout = new QVBoxLayout();
    descriptionVBoxLayout->setSpacing(6);
    descriptionVBoxLayout->setObjectName(QString::fromUtf8("descriptionVBoxLayout"));
    descriptionPlainTextEdit = new QPlainTextEdit(descriptionGroupBox);
    descriptionPlainTextEdit->setObjectName(QString::fromUtf8("descriptionPlainTextEdit"));
    descriptionPlainTextEdit->setPlainText(tr("This action exports one HTML file per note and one master index file, while also creating directories for all images and other linked attachments."));
    descriptionPlainTextEdit->setAcceptDrops(false);
    descriptionPlainTextEdit->setReadOnly(true);
    descriptionPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    descriptionVBoxLayout->addWidget(descriptionPlainTextEdit);
    verticalLayout_5->addLayout(descriptionVBoxLayout);
    mainVerticalLayout->addWidget(descriptionGroupBox);

    buttonsHorizontalLayout = new QHBoxLayout();
    buttonsHorizontalLayout->setSpacing(6);
    buttonsHorizontalLayout->setObjectName(QString::fromUtf8("buttonsHorizontalLayout"));

    #if NOT_YET
    optionsPushButton = new QPushButton(verticalLayoutWidget);
    optionsPushButton->setObjectName(QString::fromUtf8("optionsPushButton"));
    optionsPushButton->setText(tr("Options..."));
    buttonsHorizontalLayout->addWidget(optionsPushButton);
    #endif

    exportPushButton = new QPushButton(verticalLayoutWidget);
    exportPushButton->setObjectName(QString::fromUtf8("exportPushButton"));
    exportPushButton->setText(tr("&Export"));
    buttonsHorizontalLayout->addWidget(exportPushButton);

    closePushButton = new QPushButton(verticalLayoutWidget);
    closePushButton->setObjectName(QString::fromUtf8("cancelPushButton"));
    closePushButton->setText(tr("&Close"));
    buttonsHorizontalLayout->addWidget(closePushButton);

    mainVerticalLayout->addLayout(buttonsHorizontalLayout);

    this->setFont(global.getGuiFont(font()));

    connect(exportPushButton,      SIGNAL(clicked()),     this, SLOT(exportNotes()));
    connect(nnexActionRadioButton, SIGNAL(clicked()),     this, SLOT(actionRadioButtonToggled()));
    connect(htmlActionRadioButton, SIGNAL(clicked()),     this, SLOT(actionRadioButtonToggled()));
    connect(closePushButton,       SIGNAL(clicked()),     this, SLOT(close()));
}


void ExportNotesDialog::actionRadioButtonToggled () {
    QLOG_DEBUG() << "actionRadioButtonToggled";
    if (nnexActionRadioButton->isChecked())
        descriptionPlainTextEdit->setPlainText(tr("This action exports one NNEX."));
    else if (htmlActionRadioButton->isChecked())
        descriptionPlainTextEdit->setPlainText(tr("This action exports one HTML file per note and one master index file, while also creating directories for all images and other linked attachments."));
}


void ExportNotesDialog::exportNotes() {
    QLOG_DEBUG() << "exporting notes";
    if (nnexActionRadioButton->isChecked())
        dynamic_cast<NixNote *>(parent)->databaseBackup(false);
    else
        QLOG_DEBUG() << "exporting html notes";
    close();
}
