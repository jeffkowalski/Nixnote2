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

#include "ntrashtree.h"
#include "src/global.h"
#include <QHeaderView>
#include <QMouseEvent>
#include "src/sql/notetable.h"
#include "src/gui/ntrashviewdelegate.h"
#include <QMessageBox>

extern Global global;


NTrashTree::NTrashTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->count = 0;
    this->setFont(global.getGuiFont(font()));

    filterPosition = -1;
    // setup options
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setRootIsDecorated(true);
    this->setSortingEnabled(false);
    this->header()->setVisible(false);
    this->setContentsMargins(10,10,10,10);
    //this->setStyleSheet("QTreeWidget {  border: none; background-color:transparent; }");

    // Build the root item
    QIcon icon(":trashIcon");
    root = new QTreeWidgetItem(this);
    root->setIcon(0,icon);
    root->setData(0, Qt::UserRole, "root");
    root->setData(0, Qt::DisplayRole, tr("Trash"));
    QFont font = root->font(0);
    font.setBold(true);
    root->setFont(0,font);
    this->setMinimumHeight(1);
    this->addTopLevelItem(root);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(calculateHeight()));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(calculateHeight()));
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(buildSelection()));

    restoreAction = contextMenu.addAction(tr("Restore Deleted Notes"));
    connect(restoreAction, SIGNAL(triggered()), SLOT(restoreAll()));
    contextMenu.addSeparator();
    expungeAction = contextMenu.addAction(tr("Empty Trash"));
    connect(expungeAction, SIGNAL(triggered()), this, SLOT(expungeAll()));

    setItemDelegate(new NTrashViewDelegate());
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    this->setFrameShape(QFrame::NoFrame);

    QString css = global.getThemeCss("trashTreeCss");
    if (css!="")
        this->setStyleSheet(css);
}



void NTrashTree::calculateHeight()
{
    int h = 0;

    int topLevelCount = topLevelItemCount();

    for(int i = 0;i < topLevelCount;i++)    {
        QTreeWidgetItem * item = topLevelItem(i);
        h += calculateHeightRec(item);
        h += item->sizeHint(0).height() + 5;
    }

    if(h != 0)    {
        setMinimumHeight(h+5);
        setMaximumHeight(h+5);
    }
    this->setMaximumWidth(this->sizeHint().width());
}



int NTrashTree::calculateHeightRec(QTreeWidgetItem * item)
{
    if(!item)
        return 0;

    QModelIndex index = indexFromItem(item);

    if(!item->isExpanded())
    {
        return rowHeight(index);
    }

    int h = item->sizeHint(0).height() + 2 + rowHeight(index);
    int childCount = item->childCount();
    for(int i = 0; i<childCount; i++) {
        h += calculateHeightRec(item->child(i));
    }
    return h;
}


void NTrashTree::resetSize() {
    calculateHeight();
}


// This allows for the tree item to be toggled.  If the prior item is selected again
// it is deselected.  If it is the root item, we don't permit the selection.
void NTrashTree::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QModelIndex item = indexAt(event->pos());
        bool selected = selectionModel()->isSelected(indexAt(event->pos()));
        QTreeView::mousePressEvent(event);
        if (selected) {
            selectionModel()->select(item, QItemSelectionModel::Deselect);
        }
        return;
    }
    QWidget::mousePressEvent(event);
}



//*************************************************************
// This function is called when a user selects something
// within this tree.  It also emits a signal that will
// be picked up by the main nixnote program.
//*************************************************************
void NTrashTree::buildSelection() {
    QLOG_TRACE() << "Inside NTrashTree::buildSelection()";

    // First, find out if we're already viewing history.  If we are we
    // chop off the end of the history & start a new one
    if (global.filterPosition+1 < global.filterCriteria.size()) {
        while (global.filterPosition+1 < global.filterCriteria.size())
            global.filterCriteria.removeLast();
    }

    filterPosition++;
    FilterCriteria *newFilter = new FilterCriteria();
    global.filterCriteria.push_back(newFilter);
    global.filterPosition++;


    if (root->isSelected())
        newFilter->setDeletedOnly(true);
    else
        newFilter->setDeletedOnly(false);
    newFilter->resetSavedSearch = true;
    newFilter->resetDeletedOnly = true;
    newFilter->resetAttribute = true;
    newFilter->resetFavorite = true;
    newFilter->resetNotebook =true;
    newFilter->resetTags = true;
    newFilter->resetSearchString = true;

    emit updateSelectionRequested();

    QLOG_TRACE() << "Leaving NTrashTree::buildSelection()";
}


//*************************************************************
// This function is called from the main NixNote class.
// it will reset the items which are selected based upon
// what the user did somewhere else (outside this widget).
//*************************************************************
void NTrashTree::updateSelection() {
    blockSignals(true);

    FilterCriteria *criteria = global.getCurrentCriteria();
    if (global.filterPosition != filterPosition) {
        QList<QTreeWidgetItem*> selectedItems = this->selectedItems();
        for (int i=0; i<selectedItems.size() && criteria->resetDeletedOnly; i++) {
            selectedItems[i]->setSelected(false);
        }

        if (criteria->isDeletedOnlySet() && criteria->getDeletedOnly()) {
            root->setSelected(true);
        }
    }
    filterPosition = global.filterPosition;

    blockSignals(false);
}





void NTrashTree::contextMenuEvent(QContextMenuEvent *event) {
    contextMenu.exec(event->globalPos());
}




//***********************************************
//* Restore all notes from the trash
//***********************************************
void NTrashTree::restoreAll() {
    NoteTable ntable(global.db);
    QList<qint32> lids;
    ntable.getAllDeleted(lids);
    for (int i=0; i<lids.size(); i++) {
        ntable.restoreNote(lids[i], true);
        delete global.cache[lids[i]];
        global.cache.remove(lids[i]);
    }

    emit(updateSelectionRequested());
}


//***********************************************
//* Permanently delete all notes
//***********************************************
void NTrashTree::expungeAll() {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Verify Delete"));
    msgBox.setText(tr("Are you sure you want to permanently delete these notes?"));
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int rc = msgBox.exec();
    if (rc != QMessageBox::Yes)
        return;

    NoteTable ntable(global.db);
    QList<qint32> lids;
    ntable.getAllDeleted(lids);
    for (int i=0; i<lids.size(); i++) {

        Note n;
        ntable.get(n,lids[i],false,false);
        ntable.expunge(lids[i]);
        delete global.cache[lids[i]];
        global.cache.remove(lids[i]);

        // Check to see if the note is synchronized.  If so, we
        // need to keep it to let Evernote to delete it.
        if (n.updateSequenceNum.isSet() && n.updateSequenceNum>0) {
            ntable.addToDeleteQueue(lids[i],n);
        }
    }
    emit(updateSelectionRequested());
}


//**************************************************
//* Update the trash count
//**************************************************
void NTrashTree::updateTotals(qint32 total) {
    this->count = total;
    repaint();
}



QSize NTrashTree::sizeHint() {
    return QTreeView::sizeHint();
//    QSize sz = QTreeView::sizeHint();
//    QFontMetrics fm(root->font(0));
//    QString numString =  tr("Trash  ") +
//            QString("(")+QString::number(count) +QString(")");
//    int numWidth = fm.width(numString);

//    sz.setWidth(root->icon(0).availableSizes().at(0).width()
//                + numWidth+24);  // Add some extra at the end for totals
//    QLOG_DEBUG() << sz.width();

//    return sz;
}


void NTrashTree::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const {
    // suppress unused
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(index);
    return;
}

void NTrashTree::reloadIcons() {
    root->setIcon(0,global.getIconResource(":trashIcon"));
}
