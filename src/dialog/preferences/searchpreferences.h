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



#ifndef SEARCHPREFERENCES_H
#define SEARCHPREFERENCES_H

#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>

class SearchPreferences : public QWidget
{
    Q_OBJECT
private:
    QSpinBox *weight;
    QCheckBox *syncAttachments;  // Disabled for performance reasons
    QCheckBox *indexPDF;         // Index PDFs locally?
    QCheckBox *clearSearchOnNotebook;   // Clear search text when notebook changes?
    QCheckBox *clearNotebookOnSearch;   // Clear notebook on search text changes
    QCheckBox *clearTagsOnSearch;      // Clear tag selection on search text changes
    QCheckBox *tagSelectionOr;          // "OR" tag selections.

    QCheckBox *forceSearchLowerCase;
    QCheckBox *forceSearchWithoutDiacritics;


    QCheckBox *enableBackgroundIndexing;  // Do indexing in the background by default.

public:
    explicit SearchPreferences(QWidget *parent = 0);
    QString getMinimumSearchWeight();
    void saveValues();
    
signals:
    
public slots:
    
};

#endif // SEARCHPREFERENCES_H
