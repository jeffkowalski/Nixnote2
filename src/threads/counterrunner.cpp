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

#include "counterrunner.h"
#include "src/sql/notetable.h"
#include "src/sql/notebooktable.h"
#include "src/sql/nsqlquery.h"
#include "src/sql/tagtable.h"

#include <QtSql>

CounterRunner::CounterRunner(QObject *parent) :
    QObject(parent)
{
    init = false;
}


void CounterRunner::initialize() {
    if (global.countBehavior == Global::CountNone)
        return;
    init = true;
    QLOG_DEBUG() << "Starting CounterRunner";
    db = new DatabaseConnection("counterrunner");
    QLOG_DEBUG() << "CounterRunner initialization complete.";
}


void CounterRunner::countAll() {
    if (global.countBehavior == Global::CountNone)
        return;
    QLOG_TRACE_IN();
    if (!init)
        initialize();
    this->countNotebooks();
    this->countTags();
    this->countTrash();
    QLOG_TRACE_OUT();
}


void CounterRunner::countTrash() {
    if (global.countBehavior == Global::CountNone)
        return;
    QLOG_TRACE_IN();
    if (!init)
        initialize();
    NoteTable ntable(db);
    QList<qint32> lids;
    emit trashTotals(ntable.getAllDeleted(lids));
    QLOG_TRACE_OUT();
}


void CounterRunner::countNotebooks() {
    if (global.countBehavior == Global::CountNone)
        return;

    QLOG_TRACE_IN();
    if (!init)
        initialize();

    // First get every possible notebook
    NotebookTable nTable(db);
    QList<qint32> lids;
    nTable.getAll(lids);

    // Next, get the totals of everything possible
    QHash<qint32, qint32> allNotebooks;
    for (int i=0; i<lids.size(); i++) {
        allNotebooks.insert(lids.at(i), 0);
    }

    NSqlQuery query(db);
    query.exec(" select data, count(data) from datastore where key=5011 and lid not in (select lid from datastore where data=0 and key=5010) group by data;");
    while (query.next()) {
        qint32 lid = query.value(0).toInt();
        qint32 total = query.value(1).toInt();
        allNotebooks[lid] = total;
    }

    query.exec("select notebooklid, count(notebooklid) from notetable where lid in (select lid from filter) and lid not in (select lid from datastore where data=0 and key=5010) group by notebooklid;");

    while(query.next()) {
        qint32 lid = query.value(0).toInt();
        emit notebookTotals(lid, query.value(1).toInt(), allNotebooks[lid]);
        lids.removeAll(lid);
    }

    // The ones that are left have a zero count
    for (int i=0; i<lids.size(); i++)
        emit(notebookTotals(lids[i], 0, allNotebooks[lids[i]]));

    emit(notebookTotals(-1, -1, -1));
    QLOG_TRACE_OUT();
}


void CounterRunner::countTags() {
    if (global.countBehavior == Global::CountNone)
        return;
    QLOG_TRACE_IN();
    if (!init)
        initialize();
    // First get every possible tag
    TagTable tTable(db);
    QList<qint32> lids;
    tTable.getAll(lids);

    // Next, get the totals of everything possible
    QHash<qint32, qint32> allTags;
    for (int i=0; i<lids.size(); i++) {
        allTags.insert(lids.at(i), 0);
    }

    NSqlQuery query(db);
    query.exec(" select data, count(data) from datastore where key=5012 and lid not in (select lid from datastore where data=0 and key=5010) group by data;");
    while (query.next()) {
        qint32 lid = query.value(0).toInt();
        qint32 total = query.value(1).toInt();
        allTags[lid] = total;
    }

    // Start counting
    query.prepare("select data, count(lid) from datastore where key=:key and lid in (select lid from filter) and lid not in (select lid from datastore where data=0 and key=5010) group by data;");
    query.bindValue(":key", NOTE_TAG_LID);
    query.exec();
    while(query.next()) {
        qint32 lid = query.value(0).toInt();
        emit tagTotals(lid, query.value(1).toInt(), allTags[lid]);
        lids.removeAll(lid);
    }

    // The ones that are left have a zero count, so we reset them
    for (int i=0; i<lids.size(); i++)
        emit(tagTotals(lids[i], 0, allTags[lids[i]]));

    // Finally, emit that we are done so unassigned tags can be hidden
    emit(tagCountComplete());
    QLOG_TRACE_OUT();
}
