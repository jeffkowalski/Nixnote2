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

#include "ntrashviewdelegate.h"
#include <QPainter>
#include "ntrashtree.h"
#include "src/global.h"

extern Global global;

NTrashViewDelegate::NTrashViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}




void NTrashViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
#if QT_VERSION < 0x050000
    QStyleOptionViewItemV4 options = option;
#else
    QStyleOptionViewItem options = option;
#endif
    initStyleOption(&options, index);

    painter->save();

    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    if (global.countBehavior == Global::CountNone) {
        painter->restore();
        return;
    }

    NTrashTree  *tree = (NTrashTree*)options.widget;
    qint32 count = tree->count;
    QString countString = QString("(")+QString::number(count) + QString(")");

    // shift text right to make icon visible
    QSize iconSize = options.icon.actualSize(options.rect.size());
    painter->translate(options.rect.left()+iconSize.width(), options.rect.top());
    QRect clip(0, 0, options.rect.width()+iconSize.width(), options.rect.height());

    painter->setClipRect(clip);
    options.font.setBold(true);
    QFontMetrics fm = options.fontMetrics;
    painter->setPen(Qt::darkGray);
    painter->drawText(10+fm.width(index.data().toString()+QString(" ")), fm.ascent(),countString);  // Maybe fm.ascent?
//    painter->drawText(6+fm.width(index.data().toString()+QString("   ")),fm.height()-1,countString);  // Maybe fm.ascent?
    painter->restore();
}

