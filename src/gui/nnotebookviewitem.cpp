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

#include "nnotebookviewitem.h"
#include "src/sql/sharednotebooktable.h"
#include "src/sql/linkednotebooktable.h"
#include "src/sql/notebooktable.h"


/*
 * These constructors are somewhat confusing
 * If they are like they should be, a comment about the choices here
 * would be really helpful
 */

NNotebookViewItem::NNotebookViewItem(qint32 lid, QTreeWidget* parent):QTreeWidgetItem(parent) {
    total = 0;
    subTotal = 0;
    this->lid = 0;
    this->lid = lid;
    this->setIconType();
}


NNotebookViewItem::NNotebookViewItem(qint32 lid):QTreeWidgetItem(){
    subTotal = 0;
    total = 0;
    this->lid = lid;
    this->setIconType();
}

void NNotebookViewItem::setRootColor(bool val) {
    if (val) {
        QFont f;
        f.setBold(true);
        setFont(0, f);
    }
}


// Custom sort
bool NNotebookViewItem::operator<(const QTreeWidgetItem &other)const {

    // We sort two ways.  First we see if the type is a stack for a linked notebook
    // If one is a linked stack and the other is not, the one that is not wins.  This
    // makes the linked stack appear at the bottom of the list.
    int column = treeWidget()->sortColumn();
    NNotebookViewItem *ptr = (NNotebookViewItem*)&other;
    if (this->type == LinkedStack && ptr->type != LinkedStack)
        return false;

    // Otherwise we just sort by name.
    return text(column).toLower() < other.text(column).toLower();
}

void NNotebookViewItem::reloadIcon() {
    setType(this->type);
}


void NNotebookViewItem::setType(NNotebookWidgetType type) {
    this->type = type;
    if (type == Linked)
        setIcon(0, global.getIconResource(":notebookLinkedIcon"));
    if (type == Shared)
        setIcon(0, global.getIconResource(":notebookSharedIcon"));
    if (type == Local)
        setIcon(0, global.getIconResource(":notebookLocalIcon"));
    if (type == Synchronized)
        setIcon(0, global.getIconResource(":notebookSmallIcon"));
    if (type == Stack)
        setIcon(0, global.getIconResource(":stackIcon"));
    if (type == LinkedStack)
        setIcon(0, global.getIconResource(":silhouetteIcon"));
    if (type == Conflict)
        setIcon(0, global.getIconResource(":notebookConflictIcon"));
}


void NNotebookViewItem::setIconType() {
    LinkedNotebookTable linkedTable(global.db);
    SharedNotebookTable sharedTable(global.db);
    NotebookTable bookTable(global.db);

    if (lid == 0) {
        this->setType(Stack);
        return;
    }
    if (linkedTable.exists(lid)) {
        this->setType(Linked);
        return;
    }

    if (sharedTable.exists(lid)) {
        this->setType(Shared);
        return;
    }
    if (!bookTable.isLocal(lid)) {
        this->setType(Synchronized);
        return;
    }

    Notebook notebook;
    bookTable.get(notebook, lid);
    QString notebookname = "";
    if (notebook.name.isSet())
        notebookname = notebook.name;
    if (notebookname.startsWith("Conflict", Qt::CaseInsensitive)) {
        this->setType(Conflict);
        return;
    }
    this->setType(Local);
    return;

}



