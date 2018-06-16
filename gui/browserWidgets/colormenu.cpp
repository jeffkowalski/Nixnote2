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

#include "colormenu.h"
#include "global.h"

extern Global global;

ColorMenu::ColorMenu(QObject *parent) :
    QObject(parent)
{
    this->parent = parent;
    currentColor.setNamedColor("black");
    populateList();
    QString css = global.getThemeCss("colorMenuCss");
    if (css!="")
        this->menu.setStyleSheet(css);

}

QStringList ColorMenu::colorNames() {
    QStringList colors;
    colors << "black";
    colors << "gray";
    colors << "darkGrey";

    colors << "red";
    colors << "magenta";
    colors << "darkMagenta";
    colors << "darkRed";

    colors << "green";
    colors << "darkGreen";

    colors << "blue";
    colors << "darkBlue";
    colors << "cyan";
    colors << "darkCyan";

    colors << "yellow";
    colors << "white";
    return colors;
}


void ColorMenu::populateList() {
    // note: menu is created at beginning (not at runtime)
    QStringList list = colorNames();
    for (int i=0; i<list.size(); i++) {
        QPixmap pix(QSize(22,22));
        pix.fill(QColor(list[i]));
        QAction *newAction = new QAction(QIcon(pix), "", parent);
        newAction->setToolTip(list[i]);
        newAction->setText(list[i]);
        menu.addAction(newAction);
        connect(newAction, SIGNAL(hovered()), this, SLOT(itemHovered()));
    }
    QLOG_DEBUG() << "Done: populating colormenu";
}




QColor *ColorMenu::getColor() {
    return &currentColor;
}


QMenu *ColorMenu::getMenu() {
    return &menu;
}



void ColorMenu::setDefault(QColor color) {
    currentColor = color;
}

void ColorMenu::itemHovered() {
    if (menu.activeAction() != NULL && menu.activeAction()->toolTip() != NULL) {
        currentColor.setNamedColor(menu.activeAction()->toolTip());
    }
}
