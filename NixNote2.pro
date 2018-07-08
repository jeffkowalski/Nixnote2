# currently additionally  PKG_CONFIG_PATH=$$PWD/../libs/lib/pkgconfig is needed


QT       += core gui widgets printsupport webkit webkitwidgets sql network xml dbus qml
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += poppler-qt5 libcurl tidy
    QMAKE_RPATHDIR += $$PWD/../libs/lib
}

unix:!mac:LIBS += -lpthread -g -rdynamic

win32:INCLUDEPATH += "$$PWD/winlib/includes/poppler/qt5"
win32:INCLUDEPATH += "$$PWD/winlib/includes"
win32:LIBS += -L"$$PWD/winlib" -lpoppler-qt5
win32:RC_ICONS += "$$PWD/images/windowIcon.ico"


mac {
    TARGET = NixNote2
} else {
    TARGET = nixnote2
}
TEMPLATE = app
RESOURCES = NixNote2.qrc

UI_DIR = .

CONFIG(debug, debug|release) {
    DESTDIR = qmake-build-debug
    message(Debug build!)
} else {
    DESTDIR = qmake-build-release
    message(Release build!)
}
OBJECTS_DIR = $${DESTDIR}
MOC_DIR = $${DESTDIR}

TRANSLATIONS = \
    translations/nixnote2_cs_CZ.ts \
    translations/nixnote2_de.ts    \
    translations/nixnote2_en_GB.ts \
    translations/nixnote2_ca.ts    \
    translations/nixnote2_da.ts    \
    translations/nixnote2_es.ts    \
    translations/nixnote2_ja.ts    \
    translations/nixnote2_pt.ts    \
    translations/nixnote2_sk.ts    \
    translations/nixnote2_zh_TW.ts \
    translations/nixnote2_fr.ts    \
    translations/nixnote2_pl.ts    \
    translations/nixnote2_ru.ts    \
    translations/nixnote2_zh_CN.ts


SOURCES += \
    main.cpp \
    nixnote.cpp \
    global.cpp \
    settings/filemanager.cpp \
    gui/nwebpage.cpp \
    gui/nwebview.cpp \
    sql/databaseconnection.cpp \
    logger/qsdebugoutput.cpp \
    logger/qslog.cpp \
    logger/qslogdest.cpp \
    sql/configstore.cpp \
    gui/ntableview.cpp \
    gui/ntableviewheader.cpp \
    threads/syncrunner.cpp \
    sql/datastore.cpp \
    sql/usertable.cpp \
    sql/tagtable.cpp \
    sql/searchtable.cpp \
    gui/nsearchview.cpp \
    models/notemodel.cpp \
    gui/nmainmenubar.cpp \
    gui/nsearchviewitem.cpp \
    gui/ntagview.cpp \
    gui/ntagviewitem.cpp \
    gui/widgetpanel.cpp \
    gui/numberdelegate.cpp \
    gui/ntabwidget.cpp \
    gui/nnotebookviewitem.cpp \
    gui/nnotebookview.cpp \
    gui/datedelegate.cpp \
    models/ntreemodel.cpp \
    sql/resourcetable.cpp \
    sql/notetable.cpp \
    sql/notebooktable.cpp \
    filters/notesortfilterproxymodel.cpp \
    html/thumbnailer.cpp \
    html/noteformatter.cpp \
    settings/startupconfig.cpp \
    dialog/logindialog.cpp \
    gui/lineedit.cpp \
    gui/nattributetree.cpp \
    filters/filtercriteria.cpp \
    gui/ntrashtree.cpp \
    filters/filterengine.cpp \
    models/notecache.cpp \
    gui/nbrowserwindow.cpp \
    threads/indexrunner.cpp \
    html/tagscanner.cpp \
    xml/importdata.cpp \
    sql/notemetadata.cpp \
    sql/sharednotebooktable.cpp \
    gui/browserWidgets/ntitleeditor.cpp \
    gui/browserWidgets/notebookmenubutton.cpp \
    gui/browserWidgets/expandbutton.cpp \
    gui/browserWidgets/urleditor.cpp \
    gui/browserWidgets/tageditor.cpp \
    gui/browserWidgets/tageditornewtag.cpp \
    gui/browserWidgets/tagviewer.cpp \
    gui/flowlayout.cpp \
    gui/browserWidgets/authoreditor.cpp \
    gui/browserWidgets/dateeditor.cpp \
    gui/browserWidgets/datetimeeditor.cpp \
    dialog/savedsearchproperties.cpp \
    gui/treewidgeteditor.cpp \
    dialog/tagproperties.cpp \
    dialog/notebookproperties.cpp \
    html/enmlformatter.cpp \
    utilities/encrypt.cpp \
    dialog/endecryptdialog.cpp \
    oauth/oauthtokenizer.cpp \
    oauth/oauthwindow.cpp \
    communication/communicationmanager.cpp \
    gui/browserWidgets/colormenu.cpp \
    xml/xmlhighlighter.cpp \
    utilities/mimereference.cpp \
    dialog/accountdialog.cpp \
    gui/shortcutkeys.cpp \
    dialog/insertlinkdialog.cpp \
    dialog/tabledialog.cpp \
    dialog/encryptdialog.cpp \
    dialog/insertlatexdialog.cpp \
    gui/plugins/popplerviewer.cpp \
    gui/plugins/pluginfactory.cpp \
    gui/findreplace.cpp \
    sql/linkednotebooktable.cpp \
    application.cpp \
    dialog/databasestatus.cpp \
    gui/plugins/popplergraphicsview.cpp \
    threads/counterrunner.cpp \
    gui/nnotebookviewdelegate.cpp \
    gui/ntrashviewdelegate.cpp \
    gui/ntagviewdelegate.cpp \
    watcher/filewatcher.cpp \
    sql/filewatchertable.cpp \
    watcher/filewatchermanager.cpp \
    dialog/watchfolderadd.cpp \
    dialog/watchfolderdialog.cpp \
    dialog/preferences/preferencesdialog.cpp \
    dialog/preferences/debugpreferences.cpp \
    dialog/preferences/syncpreferences.cpp \
    dialog/preferences/appearancepreferences.cpp \
    settings/accountsmanager.cpp \
    dialog/adduseraccountdialog.cpp \
    dialog/accountmaintenancedialog.cpp \
    gui/truefalsedelegate.cpp \
    gui/browserWidgets/editorbuttonbar.cpp \
    communication/communicationerror.cpp \
    dialog/screencapture.cpp \
    gui/imagedelegate.cpp \
    dialog/preferences/searchpreferences.cpp \
    html/attachmenticonbuilder.cpp \
    dialog/locationdialog.cpp \
    gui/browserWidgets/locationeditor.cpp \
    dialog/preferences/localepreferences.cpp \
    gui/reminderorderdelegate.cpp \
    gui/browserWidgets/reminderbutton.cpp \
    dialog/remindersetdialog.cpp \
    reminders/reminderevent.cpp \
    reminders/remindermanager.cpp \
    dialog/notehistoryselect.cpp \
    dialog/closenotebookdialog.cpp \
    dialog/spellcheckdialog.cpp \
    gui/externalbrowse.cpp \
    sql/nsqlquery.cpp \
    dialog/aboutdialog.cpp \
    xml/importenex.cpp \
    xml/exportdata.cpp \
    dialog/logviewer.cpp \
    dialog/htmlentitiesdialog.cpp \
    qevercloud/exceptions.cpp \
    qevercloud/http.cpp \
    qevercloud/services_nongenerated.cpp \
    qevercloud/oauth.cpp \
    qevercloud/AsyncResult.cpp \
    qevercloud/EventLoopFinisher.cpp \
    qevercloud/generated/constants.cpp \
    qevercloud/generated/services.cpp \
    qevercloud/generated/types.cpp \
    gui/traymenu.cpp \
    gui/favoritesview.cpp \
    gui/favoritesviewdelegate.cpp \
    gui/favoritesviewitem.cpp \
    sql/favoritestable.cpp \
    sql/favoritesrecord.cpp \
    filters/remotequery.cpp \
    gui/browserWidgets/fontnamecombobox.cpp \
    gui/browserWidgets/fontsizecombobox.cpp \
    utilities/pixelconverter.cpp \
    utilities/noteindexer.cpp \
    xml/batchimport.cpp \
    sql/databaseupgrade.cpp \
    email/emailaddress.cpp \
    email/mimeattachment.cpp \
    email/mimecontentformatter.cpp \
    email/mimefile.cpp \
    email/mimehtml.cpp \
    email/mimeinlinefile.cpp \
    email/mimemessage.cpp \
    email/mimemultipart.cpp \
    email/mimepart.cpp \
    email/mimetext.cpp \
    email/quotedprintable.cpp \
    email/smtpclient.cpp \
    dialog/preferences/emailpreferences.cpp \
    dialog/emaildialog.cpp \
    settings/colorsettings.cpp \
    utilities/debugtool.cpp \
    cmdtools/cmdlinetool.cpp \
    cmdtools/addnote.cpp \
    utilities/crossmemorymapper.cpp \
    cmdtools/cmdlinequery.cpp \
    utilities/nuuid.cpp \
    cmdtools/deletenote.cpp \
    cmdtools/emailnote.cpp \
    dialog/faderdialog.cpp \
    cmdtools/extractnotetext.cpp \
    cmdtools/extractnotes.cpp \
    cmdtools/alternote.cpp \
    cmdtools/importnotes.cpp \
    dialog/preferences/thumbnailpreferences.cpp \
    dialog/noteproperties.cpp \
    dialog/shortcutdialog.cpp \
    cmdtools/signalgui.cpp \
    gui/browserWidgets/table/tablepropertiesdialog.cpp \
    threads/browserrunner.cpp \
    exits/exitpoint.cpp \
    exits/exitmanager.cpp \
    dialog/preferences/exitpreferences.cpp

HEADERS  += \
    nixnote.h \
    global.h \
    settings/filemanager.h \
    gui/nwebpage.h \
    gui/nwebview.h \
    sql/databaseconnection.h \
    logger/qsdebugoutput.h \
    logger/qslogdest.h \
    logger/qslog.h \
    sql/configstore.h \
    gui/ntableview.h \
    gui/ntableviewheader.h \
    threads/syncrunner.h \
    sql/datastore.h \
    sql/usertable.h \
    sql/tagtable.h \
    sql/searchtable.h \
    gui/nsearchview.h \
    models/notemodel.h \
    gui/nmainmenubar.h \
    gui/nsearchviewitem.h \
    gui/ntagview.h \
    gui/ntagviewitem.h \
    gui/widgetpanel.h \
    gui/numberdelegate.h \
    gui/ntabwidget.h \
    gui/nnotebookviewitem.h \
    gui/nnotebookview.h \
    gui/datedelegate.h \
    models/ntreemodel.h \
    sql/resourcetable.h \
    sql/notetable.h \
    sql/notebooktable.h \
    filters/notesortfilterproxymodel.h \
    html/thumbnailer.h \
    html/noteformatter.h \
    settings/startupconfig.h \
    dialog/logindialog.h \
    gui/lineedit.h \
    gui/nattributetree.h \
    filters/filtercriteria.h \
    gui/ntrashtree.h \
    filters/filterengine.h \
    models/notecache.h \
    gui/nbrowserwindow.h \
    threads/indexrunner.h \
    html/tagscanner.h \
    xml/importdata.h \
    sql/notemetadata.h \
    sql/sharednotebooktable.h \
    gui/browserWidgets/ntitleeditor.h \
    gui/browserWidgets/notebookmenubutton.h \
    gui/browserWidgets/expandbutton.h \
    gui/browserWidgets/urleditor.h \
    gui/browserWidgets/tageditor.h \
    gui/browserWidgets/tageditornewtag.h \
    gui/browserWidgets/tagviewer.h \
    gui/flowlayout.h \
    gui/browserWidgets/authoreditor.h \
    gui/browserWidgets/dateeditor.h \
    gui/browserWidgets/datetimeeditor.h \
    dialog/savedsearchproperties.h \
    gui/treewidgeteditor.h \
    dialog/tagproperties.h \
    dialog/notebookproperties.h \
    html/enmlformatter.h \
    utilities/encrypt.h \
    dialog/endecryptdialog.h \
    oauth/oauthtokenizer.h \
    oauth/oauthwindow.h \
    communication/communicationmanager.h \
    gui/browserWidgets/colormenu.h \
    xml/xmlhighlighter.h \
    utilities/mimereference.h \
    dialog/accountdialog.h \
    gui/shortcutkeys.h \
    dialog/insertlinkdialog.h \
    dialog/tabledialog.h \
    dialog/encryptdialog.h \
    dialog/insertlatexdialog.h \
    gui/plugins/popplerviewer.h \
    gui/plugins/pluginfactory.h \
    gui/findreplace.h \
    sql/linkednotebooktable.h \
    application.h \
    dialog/databasestatus.h \
    gui/plugins/popplergraphicsview.h \
    threads/counterrunner.h \
    gui/nnotebookviewdelegate.h \
    gui/ntrashviewdelegate.h \
    gui/ntagviewdelegate.h \
    watcher/filewatcher.h \
    sql/filewatchertable.h \
    watcher/filewatchermanager.h \
    dialog/watchfolderadd.h \
    dialog/watchfolderdialog.h \
    dialog/preferences/preferencesdialog.h \
    dialog/preferences/syncpreferences.h \
    settings/accountsmanager.h \
    dialog/adduseraccountdialog.h \
    dialog/accountmaintenancedialog.h \
    gui/truefalsedelegate.h \
    gui/browserWidgets/editorbuttonbar.h \
    communication/communicationerror.h \
    dialog/screencapture.h \
    gui/imagedelegate.h \
    dialog/preferences/searchpreferences.h \
    html/attachmenticonbuilder.h \
    dialog/locationdialog.h \
    gui/browserWidgets/locationeditor.h \
    dialog/preferences/localepreferences.h \
    gui/reminderorderdelegate.h \
    gui/browserWidgets/reminderbutton.h \
    dialog/remindersetdialog.h \
    reminders/reminderevent.h \
    reminders/remindermanager.h \
    dialog/notehistoryselect.h \
    dialog/closenotebookdialog.h \
    dialog/spellcheckdialog.h \
    gui/externalbrowse.h \
    sql/nsqlquery.h \
    dialog/aboutdialog.h \
    xml/importenex.h \
    xml/exportdata.h \
    dialog/logviewer.h \
    dialog/htmlentitiesdialog.h \
    qevercloud/exceptions.h \
    qevercloud/globals.h \
    qevercloud/http.h \
    qevercloud/impl.h \
    qevercloud/oauth.h \
    qevercloud/public.h \
    qevercloud/thrift.h \
    qevercloud/thumbnail.h \
    qevercloud/AsyncResult.h \
    qevercloud/EventLoopFinisher.h \
    qevercloud/EverCloudException.h \
    qevercloud/Optional.h \
    qevercloud/qt4helpers.h \
    qevercloud/generated/constants.h \
    qevercloud/generated/services.h \
    qevercloud/generated/types.h \
    qevercloud/generated/types_impl.h \
    qevercloud/generated/EDAMErrorCode.h \
    qevercloud/include/QEverCloud.h \
    qevercloud/include/QEverCloudOAuth.h \
    gui/traymenu.h \
    gui/favoritesview.h \
    gui/favoritesviewdelegate.h \
    gui/favoritesviewitem.h \
    sql/favoritestable.h \
    sql/favoritesrecord.h \
    filters/remotequery.h \
    gui/browserWidgets/fontnamecombobox.h \
    gui/browserWidgets/fontsizecombobox.h \
    utilities/pixelconverter.h \
    utilities/noteindexer.h \
    xml/batchimport.h \
    sql/databaseupgrade.h \
    email/emailaddress.h \
    email/mimeattachment.h \
    email/mimecontentformatter.h \
    email/mimefile.h \
    email/mimehtml.h \
    email/mimeinlinefile.h \
    email/mimemessage.h \
    email/mimemultipart.h \
    email/mimepart.h \
    email/mimetext.h \
    email/quotedprintable.h \
    email/smtpclient.h \
    email/smtpexports.h \
    dialog/preferences/emailpreferences.h \
    dialog/emaildialog.h \
    settings/colorsettings.h \
    utilities/debugtool.h \
    cmdtools/cmdlinetool.h \
    cmdtools/addnote.h \
    utilities/crossmemorymapper.h \
    cmdtools/cmdlinequery.h \
    utilities/nuuid.h \
    cmdtools/deletenote.h \
    cmdtools/emailnote.h \
    dialog/faderdialog.h \
    cmdtools/extractnotetext.h \
    cmdtools/extractnotes.h \
    cmdtools/alternote.h \
    cmdtools/importnotes.h \
    plugins/webcam/webcaminterface.h \
    plugins/hunspell/hunspellinterface.h \
    dialog/preferences/thumbnailpreferences.h \
    dialog/noteproperties.h \
    dialog/shortcutdialog.h \
    cmdtools/signalgui.h \
    gui/browserWidgets/table/tablepropertiesdialog.h \
    dialog/preferences/appearancepreferences.h \
    dialog/preferences/debugpreferences.h \
    threads/browserrunner.h \
    exits/exitpoint.h \
    exits/exitmanager.h \
    dialog/preferences/exitpreferences.h

linux:QMAKE_CXXFLAGS += -std=c++11 -g -O2 -fstack-protector-strong -Wformat -Werror=format-security
linux:QMAKE_LFLAGS += -Wl,-Bsymbolic-functions -Wl,-z,relro

win32:QMAKE_CXXFLAGS +=-g -O2 --param=ssp-buffer-size=4 -Wformat -Werror=format-security
win32:QMAKE_LFLAGS += -Wl,-Bsymbolic-functions
win32:DEFINES += SMTP_BUILD

isEmpty(PREFIX) {
 PREFIX = /usr/local
}

binary.path = $${PREFIX}/bin
binary.files = $${DESTDIR}/$${TARGET}
message("Target binary: $${binary.files}")

desktop.path = $${PREFIX}/share/applications
desktop.files = $${TARGET}.desktop

images.path = $${PREFIX}/share/$$TARGET/images
images.files = images/*

java.path = $${PREFIX}/share/$$TARGET/java
java.files = java/*

help.path = $${PREFIX}/share/$$TARGET/help
help.files = help/*

resources.path = $${PREFIX}/share/$$TARGET
resources.files = $$PWD/shortcuts.txt $$PWD/themes.ini $$PWD/LICENSE $$PWD/colors.txt \
                  $${DESTDIR}/build-version.txt $$PWD/version.txt

# compile the translation files:
isEmpty(QMAKE_LRELEASE) {
    win32:LANGREL = $$[QT_INSTALL_BINS]\lrelease.exe
    else:LANGREL = $$[QT_INSTALL_BINS]/lrelease
}
TRANSLATION_TARGET_DIR = $${OUT_PWD}/translations
langrel.input = TRANSLATIONS
langrel.output = $$TRANSLATION_TARGET_DIR/${QMAKE_FILE_BASE}.qm
langrel.commands = \
    $$LANGREL -compress -nounfinished -removeidentical ${QMAKE_FILE_IN} -qm $$TRANSLATION_TARGET_DIR/${QMAKE_FILE_BASE}.qm
langrel.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += langrel
# this launches the actual work:
PRE_TARGETDEPS += compiler_langrel_make_all


mac {
    # TODO 6.2018 this will need minor adjustments

    ICON = images/NixNote2.icns

    # we go for an appbundle that contains all resources (except
    # the shared library dependencies - use macdeployqt for those).
    images.path = Contents/Resources
    images.files = images
    java.path = Contents/Resources
    java.files = java
    mactranslations.path = Contents/Resources/translations
    mactranslations.files = $$files($$TRANSLATION_TARGET_DIR/*.qm)
    mactranslations.depends = compiler_langrel_make_all

    help.path = Contents/Resources
    help.files = help
    QMAKE_BUNDLE_DATA += images java mactranslations help
    INSTALLS = binary
} else {
    translations.path = $${PREFIX}/share/$$TARGET/translations
    translations.files = $$files($$TRANSLATION_TARGET_DIR/*.qm)
    INSTALLS = binary desktop images java translations help resources
}
