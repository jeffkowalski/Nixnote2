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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>


#include "src/dialog/preferences/debugpreferences.h"
#include "src/dialog/preferences/syncpreferences.h"
#include "src/dialog/preferences/appearancepreferences.h"
#include "src/dialog/preferences/searchpreferences.h"
#include "src/dialog/preferences/localepreferences.h"
#include "src/dialog/preferences/emailpreferences.h"
#include "src/dialog/preferences/thumbnailpreferences.h"
#include "src/dialog/preferences/exitpreferences.h"


class PreferencesDialog : public QDialog
{
    Q_OBJECT
private:
    QTabWidget *tabs;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    void setupDebugPanel();
    void setupSyncPanel();
    void setupAppearancePanel();
    void setupLocalePanel();
    void setupSearchPanel();
    void setupEmailPanel();
    void setupThumbnailPanel();
    void setupExitPanel();

    DebugPreferences *debugPanel;
    EmailPreferences *emailPanel;
    SyncPreferences *syncPanel;
    SearchPreferences *searchPanel;
    LocalePreferences *localePanel;
    ThumbnailPreferences *thumbnailPanel;
    AppearancePreferences *appearancePanel;
    ExitPreferences *exitPanel;
    QPushButton *okButton;
    QPushButton *cancelButton;

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();
    bool okButtonPressed;
    
signals:
    
public slots:

private slots:
    void cancelButtonClicked();
    void okButtonClicked();
    
};

#endif // PREFERENCESDIALOG_H
