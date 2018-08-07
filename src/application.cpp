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

#include "src/application.h"
#include "src/global.h"
#include <QDebug>

// Windows Check
#ifndef _WIN32
#include <execinfo.h>
#endif // End windows check

extern Global global;

Application::Application(int &argc, char** argv) :
    QApplication(argc, argv)
{
}


bool Application::notify(QObject *receiver_, QEvent *event_)
{
  try
  {
    return QApplication::notify(receiver_, event_);
  }
  catch (std::exception &ex)
  {     
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "**********************************************************";
        QLOG_ERROR() << "std::exception was caught: " << ex.what();

// Windows Check
#ifndef _WIN32
        void *array[30];
        size_t size;

        // get void*'s for all entries on the stack
        size = backtrace(array, 30);

        // print out all the frames to stderr
        backtrace_symbols_fd(array, size, 2);
#endif  // End windows check

        emit (stdException(QString::fromStdString(ex.what())));
  }

  return false;
}
