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

#include "tagviewer.h"
#include "src/global.h"

#include <QIcon>
#include <QFontMetrics>
#include <QStyle>

extern Global global;

TagViewer::TagViewer(QWidget *parent) :
    QLabel(parent)
{

    clearButton = new QToolButton(this);
    clearButton->setIcon(global.getIconResource(":filecloseIcon"));
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    clearButton->setCursor(Qt::PointingHandCursor);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    QString css = global.getThemeCss("noteTagViewerCss");
    if (css!="")
        this->setStyleSheet(css);
    else
        setStyleSheet("QFrame{border: 1px solid darkgray; border-radius: 8px;}");

    this->setFont(global.getGuiFont(font()));
}


void TagViewer::resizeEvent(QResizeEvent *)
{
    resize();
}



void TagViewer::hideEvent(QHideEvent* event) {
    Q_UNUSED(event);  // suppress unused
    hide();
    clearButton->hide();
}

void TagViewer::showEvent(QShowEvent* event) {
    Q_UNUSED(event);  // suppress unused
    show();
    clearButton->show();
}


void TagViewer::resize() {
    QFontMetrics fm(font());
    int width = fm.width(text());

    int x = width + clearButton->minimumWidth()+20;
    setMinimumWidth(x);
    setMinimumWidth(x);

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
#ifndef __WIN32
    int height = fm.descent();
    clearButton->move(rect().left() + width + frameWidth,
                      (rect().top()- height)/2);
#else
    clearButton->move(rect().left() + width + frameWidth,
                      (rect().top()- (fm.height()/2))/2);
#endif

}


void TagViewer::buttonClicked() {
    emit(closeClicked(text()));
}

