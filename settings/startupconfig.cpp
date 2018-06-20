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

#include "startupconfig.h"
#include <QDir>
#include <QString>
#include <iostream>
#include "threads/syncrunner.h"
#include "global.h"

#include <QProcessEnvironment>

extern Global global;

StartupConfig::StartupConfig()
{
    // we first allow command line override and set-up final directory later in global.setup()
    configDir = QString("");
    programDataDir = QString("");
    this->logLevel = -1; // default: not set by command line
    this->forceNoStartMinimized = false;
    this->startupNewNote = false;
    this->sqlExec = false;
    this->sqlString = "";
    this->forceStartMinimized = false;
    this->enableIndexing = false;
    this->startupNoteLid = 0;
    this->forceSystemTrayAvailable=false;
    this->disableEditing = false;
    this->accountId=-1;

    command = new QBitArray(STARTUP_OPTION_COUNT);
    command->fill(false);
    newNote = NULL;
    queryNotes = NULL;
    purgeTemporaryFiles=true;
    delNote = NULL;
    email = NULL;
    extractText = NULL;
    exportNotes = NULL;
    importNotes = NULL;
    alter = NULL;
    signalGui = NULL;
}



// Print out user help
void StartupConfig::printHelp() {
    QString help = QString("usage: nixnote2 <command>\n"
                   +QString("  <command> options:\n\n")
                   +QString("  help or ? or --? or --help           Show this message\n")
                   +QString("  start <options>                      Start NixNote GUI with the specified options.\n")
                   +QString("                                       If no command is specified, this is the default.\n")
                   +QString("     start options:\n")
                   +QString("          --logLevel=<level>           Set initial logging level (0=trace,1=debug,2=info,3=error,..\n")
                   +QString("                                       This is ONLY valid at program startup until settings are read.\n")
                   +QString("          --accountId=<id>             Start with specified user account.\n")

                   // see FileManager.getConfigDir() for more info
                   +QString("          --configDir=<dir>            Directory containing config files.\n")
                   +QString("          --userDataDir=<dir>          Directory containing database, logs etc..\n")
                   +QString("                                       Warning: ff you set configDir, but don't set userDataDir; userDataDir defaults\n")
                   +QString("                                       to configDir.\n")
                   +QString("          --programDataDir=<dir>       Directory containing deployed fixed program data (like images).\n")

                   +QString("          --dontStartMinimized         Override option to start minimized.\n")
                   +QString("          --disableEditing             Disable note editing\n")
                   +QString("          --enableIndexing             Enable background Indexing (can cause problems)\n")
                   +QString("          --openNote=<lid>             Open a specific note on startup\n")
                   +QString("          --forceSystemTrayAvailable   Force the program to accept that\n")
                   +QString("                                       the desktop supports tray icons.\n")
                   +QString("          --startMinimized             Force a startup with NixNote minimized\n")
                   +QString("          --syncAndExit                Synchronize and exit the program.\n")
                   +QString("  sync                                 Synchronize with Evernote without showing GUI.\n")
                   +QString("  shutdown                             If running, ask NixNote to shutdown\n")
                   +QString("  show_window                          If running, ask NixNote to show the main window.\n")
                   +QString("  query <options>                      If running, search NixNote and display the results.\n")
                   +QString("     query options:\n")
                   +QString("          --search=\"search string\"     Search string.\n\n")
                   +QString("          --delimiter=\"character\"      Character to place between fields.  Defaults to |.\n")
                   +QString("          --noHeaders                  Do not show column headings.")
                   +QString("          --display=\"<output format>\"  Search string.\n\n")
                   +QString("             Output Format: <fieldID><padding><:>\n")
                   +QString("                %i                     Show the internal note ID.\n")
                   +QString("                %t                     Show the note title.\n")
                   +QString("                %n                     Show the notebook.\n")
                   +QString("                %g                     Show tags.\n")
                   +QString("                %c                     Show the date the note was created.\n")
                   +QString("                %u                     Show the last date updated.\n")
                   +QString("                %e                     Show if is synchronized with Evernote.\n")
                   +QString("                %s                     Show the source URL.\n")
                   +QString("                %a                     Show the author.\n")
                   +QString("                %x                     Show if the note has a todo item.\n")
                   +QString("                %r                     Show the reminder time.\n")
                   +QString("                %v                     Show the time the reminder was completed.\n")
                   +QString("                <padding>              Pad the field to this number of spaces on the display.\n")
                   +QString("                <:>                    Truncate the field if longer than the padding.\n")
                   +QString("  addNote <options>                    Add a new note via the command line.\n")
                   +QString("     addNote options:\n")
                   +QString("          --title=\"<title>\"            Title of the new note.\n")
                   +QString("          --notebook=\"<notebook>\"      Notebook for the new note.\n")
                   +QString("          --tag=\"<tag>\"                Assign a tag.\n")
                   +QString("                                       For multiple tags use multiple --tag statements.\n")
                   +QString("          --attachment=\"<file_path>\"   File to attach to the note.\n")
                   +QString("                                       For multiple files, use multiple --attachment statements.\n")
                   +QString("          --delimiter=\"<delmiiter>\"    Character string identifying attachment points.\n")
                   +QString("                                       Defaults to %%.\n")
                   +QString("          --created=\"<datetime>\"   Date & time created in yyyy-MM-ddTHH:mm:ss.zzzZ format.\n")
                   +QString("          --updated=\"<datetime>\"   Date & time updated in yyyy-MM-ddTHH:mm:ss.zzzZ format.\n")
                   +QString("          --reminder=\"<datetime>\"  Reminder date & time in yyyy-MM-ddTHH:mm:ss.zzzZ format.\n")
                   +QString("          --noteText=\"<text>\"          Text of the note.  If not provided input\n")
                   +QString("                                       is read from stdin.\n")
                   +QString("  appendNote <options>                 Append to an existing note.\n")
                   +QString("     appendNote options:\n")
                   +QString("          --id=\"<title>\"               ID of note to append.\n")
                   +QString("          --attachment=\"<file_path>\"   File to attach to the note.\n")
                   +QString("                                       For multiple files, use multiple --attachment statements.\n")
                   +QString("          --delimiter=\"<delmiiter>\"    Character string identifying attachment points.\n")
                   +QString("                                       Defaults to %%.\n")
                   +QString("          --noteText=\"<text>\"          Text of the note.  If not provided input\n")
                   +QString("                                       is read from stdin.\n")
                   +QString("  alterNote <options>                  Change a notes't notebook or tags.\n")
                   +QString("     alterNote options:\n")
                   +QString("          --id=\"<note_ids>\"            Space separated list of note IDs to extract.\n")
                   +QString("          --search=\"search string\"     Alter notes matching search string.\n")
                   +QString("          --notebook=\"<notebook>\"      Move matching notes to this notebook.\n")
                   +QString("          --addTag=\"<tag_name>\"        Add this tag to matching notes.\n")
                   +QString("          --delTag=\"<tag_name>\"        Remove this tag from matching notes.\n")
                   +QString("          --reminder=\"<datetime>\"      Set a reminder in yyyy-MM-ddTHH:mm:ss.zzzZ format")
                   +QString("          --reminderClear              Clear the note's reminder.\n")
                   +QString("          --reminderComplete           Set the reminder as complete.\n")
                   +QString("                                       yyyy-MM-ddTHH:mm:ss.zzzZ format or the literal 'now' to default\n")
                   +QString("                                       to the current date & time.")
                   +QString("  readNote <options>                   Read the text contents of a note.\n")
                   +QString("          --id=\"<note_id>\"             ID of the note to read.\n")
                   +QString("  deleteNote <options>                 Move a note to the trash via the command line.\n")
                   +QString("     deleteNote options:\n")
                   +QString("          --id=\"<note_id>\"             ID of the note to delete.\n")
                   +QString("          --noVerify                   Do not prompt for verification.\n")
                   +QString("  emailNote <options>                  Move a note to the trash via the command line.\n")
                   +QString("     emailNote options:\n")
                   +QString("          --id=\"<note_id>\"             ID of the note to email.\n")
                   +QString("          --subject=\"<subject>\"        Additional comments.\n")
                   +QString("          --to=\"<address list>\"        List of recipients for the email.\n")
                   +QString("          --cc=\"<address list>\"        List of recipients to carbon copy.\n")
                   +QString("          --bcc=\"<address list>\"       List of recipients to blind carbon copy.\n")
                   +QString("          --note=\"<note>\"              Additional comments.\n")
                   +QString("          --ccSelf                     Send a copy to yourself.\n")
                   +QString("  backup <options>                     Backup the NixNote database.\n")
                   +QString("     backup options:\n")
                   +QString("          --output=<filename>          Output filename.\n")
                   +QString("  export <options>                     Export notes from NixNote.\n")
                   +QString("     export options:\n")
                   +QString("          --id=\"<note_ids>\"            Space separated list of note IDs to extract.\n")
                   +QString("          --search=\"search string\"     Export notes matching search string.\n")
                   +QString("          --output=\"filename\"        Output file name.\n")
                   +QString("          --deleteAfterExtract         Delete notes after the extract completes.\n")
                   +QString("          --noVerifyDelete             Don't verify deletions.\n")
                   +QString("  import <options>                     Import notes from a NixNote extract (.nnex).\n")
                   +QString("     import options:\n")
                   +QString("          --input=\"filename\"         Input file name.\n")
                   +QString("  closeNotebook <options>              Close a notebook.\n")
                   +QString("     closeNotebook options:\n")
                   +QString("          --notebook=\"notebook\"        Notebook name.\n")
                   +QString("  openNotebook <options>               Open a closed a notebook.\n")
                   +QString("     openNotebook options:\n")
                   +QString("          --notebook=\"notebook\"        Notebook name.\n")
                   +QString("  signalGui <options>                  Send command to a running NixNote.\n")
                   +QString("     signalGui options:\n")
                   +QString("          --show                       Show NixNote if hidden.\n")
                   +QString("          --synchronize                Synchronize with Evernote.\n")
                   +QString("          --shutdown                   Shutdown NixNote.\n")
                   +QString("          --screenshot                 Start a screen capture.\n")
                   +QString("          --openNote                   Open a note.  --id=<id> must be specified.\n")
                   +QString("          --openNoteNewTab             Open a note in a new tab.  --id=<id> must be specified.\n")
                   +QString("          --openExternalNote           Open a note in an external window.  --id=<id> must be specified.\n")
                   +QString("          --id=<id>                    Note Id to open.\n")
                   +QString("          --newNote                    Create a new note.\n")
                   +QString("          --newExternalNote            Create a new note in an external window.\n")
                   +QString("  Examples:\n\n")
                   +QString("     To start NixNote, do a sync, and then exit.\n")
                   +QString("     nixnote2 start --syncAndExit\n\n")
                   +QString("     To start NixNote using a secondary account.\n")
                   +QString("     nixnote2 --accountId=2\n\n")
                   +QString("     To close an open notebook.\n")
                   +QString("     nixnote2 --closeNotebook notebook=\"My Notebook\"\n\n")
                   +QString("     To add a note to the notebook \"My Notebook\"\n")
                   +QString("     nixnote2 addNote --notebook=\"My Stuff\" --title=\"My New Note\" --tag=\"Tag1\" --tag=\"Tag2\" --noteText=\"My Note Text\"\n\n")
                   +QString("     To append to an existing note.\n")
                   +QString("     nixnote2 appendNote --id=3 --noteText=\"My Note Text\"\n\n")
                   +QString("     To add a tag to notes in the notebook \"Stuff\".\n")
                   +QString("     nixnote2 alterNote --search=\"notebook:Stuff\" --addTag=\"NewTag\"\n\n")
                   +QString("     Query notes for the search text. Results show the ID, note title (padded to 10 characters but truncated longer) and the notebook\n")
                   +QString("     nixnote2 query --search=\"Famous Authors\" --delimiter=\" * \" --display=\"\%i%t10:%n\"\n\n")
                   +QString("     To extract all notes in the \"Notes\" notebook.\n")
                   +QString("     nixnote2 export --search=\"notebook:notes\" --output=/home/joe/exports.nnex\n\n")
                   +QString("     To signal NixNote to do a screenshot from the command line (NixNote must already be running).\n")
                   +QString("     nixnote2 signalGui --screenshot\n\n")
                   +QString("\n\n")
                   );

    std::cout << help.toStdString(); // ok to use cout => help text
}




int StartupConfig::init(int argc, char *argv[], bool &guiAvailable) {
    guiAvailable = true;

    // Check if we have a GUI available. This is ugly, but it works.
    // We check for a DISPLAY value, if one is found then we assume
    // that the GUI is available. We can override this with the --forceNoGui
    // as any parameter.

// Windows Check
#ifndef _WIN32
    QString display = QProcessEnvironment::systemEnvironment().value("DISPLAY", "");
    if (display.trimmed() == "")
        guiAvailable = false;
#endif // End windows check

    for (int i=1; i<argc; i++) {
        QString parm(argv[i]);
        if (parm == "--help" || parm == "-?" || parm == "help" || parm == "--?") {
            printHelp();
            return 1;
        }
        if (parm.startsWith("--logLevel=", Qt::CaseSensitive)) {
            parm = parm.section('=', 1, 1); // 2nd part
            int level = parm.toInt();
            QLOG_INFO() << "Changed logLevel via command line option to " << level;
            global.setDebugLevel(level);
            this->logLevel = level;
        }
        if (parm.startsWith("--accountId=", Qt::CaseSensitive)) {
            parm = parm.section('=', 1, 1);
            accountId = parm.toInt();
            QLOG_DEBUG() << "Set accountId via command line option to " + accountId;
        }

        // directory overrides
        if (parm.startsWith("--configDir=", Qt::CaseSensitive)) {
            parm = parm.section('=', 1, 1);
            configDir = parm;
            QLOG_INFO() << "Set configDir via command line to " + configDir;
        }
        if (parm.startsWith("--programDataDir=", Qt::CaseSensitive)) {
            parm = parm.section('=', 1, 1);
            programDataDir = parm;
            QLOG_INFO() << "Set programDataDir via command line to " + programDataDir;
        }
        if (parm.startsWith("--userDataDir=", Qt::CaseSensitive)) {
            parm = parm.section('=', 1, 1);
            userDataDir = parm;
            QLOG_INFO() << "Set userDataDir via command line to " + userDataDir;
        }


        if (parm.startsWith("addNote")) {
            command->setBit(STARTUP_ADDNOTE,true);
            if (newNote == NULL)
                newNote = new AddNote();
            guiAvailable = false;
        }
        if (parm.startsWith("appendNote")) {
            command->setBit(STARTUP_APPENDNOTE,true);
            if (newNote == NULL)
                newNote = new AddNote();
            guiAvailable = false;
        }
        if (parm.startsWith("emailNote")) {
            command->setBit(STARTUP_EMAILNOTE,true);
            if (email == NULL)
                email = new EmailNote();
            guiAvailable = false;
        }
        if (parm.startsWith("export")) {
            command->setBit(STARTUP_EXPORT,true);
            if (exportNotes == NULL)
                exportNotes = new ExtractNotes();
            guiAvailable = false;
            exportNotes->backup=false;
        }
        if (parm.startsWith("import")) {
            command->setBit(STARTUP_IMPORT,true);
            if (importNotes == NULL)
                importNotes = new ImportNotes();
            guiAvailable = false;
        }
        if (parm.startsWith("backup")) {
            command->setBit(STARTUP_BACKUP,true);
            if (exportNotes == NULL)
                exportNotes = new ExtractNotes();
            exportNotes->backup=true;
            guiAvailable = false;
        }
        if (parm.startsWith("query")) {
            command->setBit(STARTUP_QUERY);
            if (queryNotes == NULL)
                queryNotes = new CmdLineQuery();
            guiAvailable = false;
        }
        if (parm.startsWith("readNote")) {
            command->setBit(STARTUP_READNOTE);
            if (extractText == NULL)
                extractText = new ExtractNoteText();
            guiAvailable = false;
        }
        if (parm.startsWith("deleteNote")) {
            command->setBit(STARTUP_DELETENOTE);
            if (delNote == NULL)
                delNote = new DeleteNote();
            guiAvailable = false;
        }
        if (parm.startsWith("sync")) {
            command->setBit(STARTUP_SYNC,true);
            guiAvailable = false;
        }
        if (parm.startsWith("show_window")) {
            command->setBit(STARTUP_SHOW,true);
        }
        if (parm.startsWith("shutdown")) {
            command->setBit(STARTUP_SHUTDOWN,true);
        }
        if (parm.startsWith("alterNote")) {
            command->setBit(STARTUP_ALTERNOTE,true);
            if (alter == NULL)
                alter = new AlterNote();
        }
        if (parm.startsWith("openNotebook")) {
            command->setBit(STARTUP_OPENNOTEBOOK);
            notebookList.clear();
        }
        if (parm.startsWith("closeNotebook")) {
            command->setBit(STARTUP_CLOSENOTEBOOK);
            notebookList.clear();
        }
        if (parm.startsWith("sqlExec", Qt::CaseSensitive)) {
            command->setBit(STARTUP_SQLEXEC);
            guiAvailable = false;
        }
        if (parm.startsWith("signalGui")) {
            command->setBit(STARTUP_SIGNALGUI,true);
            if (signalGui == NULL)
                signalGui = new SignalGui();
            guiAvailable = false;
        }

        // This should be last because it is the default
        if (parm.startsWith("start")) {
            command->setBit(STARTUP_GUI,true);
            guiAvailable = true;
        }

        if (command->at(STARTUP_ADDNOTE)) {
            if (parm.startsWith("--title=", Qt::CaseSensitive)) {
                parm = parm.mid(8);
                newNote->title = parm;
            }
            if (parm.startsWith("--notebook=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                newNote->notebook = parm;
            }
            if (parm.startsWith("--tag=", Qt::CaseSensitive)) {
                parm = parm.mid(6);
                newNote->tags.append(parm);
            }
            if (parm.startsWith("--attachment=", Qt::CaseSensitive)) {
                parm = parm.mid(13);
                newNote->attachments.append(parm);
            }
            if (parm.startsWith("--created=", Qt::CaseSensitive)) {
                parm = parm.mid(10);
                newNote->created = parm;
            }
            if (parm.startsWith("--updated=", Qt::CaseSensitive)) {
                parm = parm.mid(10);
                newNote->updated = parm;
            }
            if (parm.startsWith("--reminder=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                newNote->reminder = parm;
            }
            if (parm.startsWith("--noteText=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                newNote->content = parm;
            }
        }
        if (command->at(STARTUP_APPENDNOTE)) {
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                newNote->lid = parm.toInt();
            }
            if (parm.startsWith("--noteText=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                newNote->content = parm;
            }
        }
        if (command->at(STARTUP_QUERY)) {
            if (parm.startsWith("--search=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                queryNotes->query = parm;
            }
            if (parm.startsWith("--display=", Qt::CaseSensitive)) {
                parm = parm.mid(10);
                queryNotes->outputFormat = parm;
            }
            if (parm.startsWith("--delimiter=", Qt::CaseSensitive)) {
                parm = parm.mid(12);
                queryNotes->delimiter = parm;
            }
            if (parm.startsWith("--noHeaders", Qt::CaseSensitive)) {
                queryNotes->printHeaders=false;
            }
        }
        if (command->at(STARTUP_GUI) || command->count(true) == 0) {
            command->setBit(STARTUP_GUI,true);
            if (parm.startsWith("--openNote=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                startupNoteLid = parm.toInt();
            }
            if (parm == "--disableEditing") {
                disableEditing = true;
            }
            if (parm == "--dontStartMinimized") {
                forceNoStartMinimized = true;
            }
            if (parm == "--startMinimized") {
                forceStartMinimized = true;
            }
            if (parm == "--newNote") {
                startupNewNote = true;
            }
            if (parm == "--syncAndExit") {
                command->clear();
                command->setBit(STARTUP_SYNC, true);
            }
            if (parm == "--enableIndexing") {
                enableIndexing = true;
            }
            if (parm == "--forceSystemTrayAvailable") {
                forceSystemTrayAvailable = true;
            }
        }
        if (command->at(STARTUP_DELETENOTE)) {
            if (parm == "--noVerify") {
                delNote->verifyDelete = false;
            }
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                delNote->lid = parm.toInt();
            }
        }
        if (command->at(STARTUP_EXPORT)) {
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                QRegExp regExp("[ ,;]");
                QStringList tokens = parm.split(regExp);
                for (int i=0; i<tokens.size(); i++) {
                    if (tokens[i].trimmed() != "")
                        exportNotes->lids.append(tokens[i].toInt());
                }
            }
            if (parm.startsWith("--deleteAfterExport", Qt::CaseSensitive)) {
                exportNotes->deleteAfterExtract=true;
            }
            if (parm.startsWith("--noVerifyDelete", Qt::CaseSensitive)) {
                exportNotes->verifyDelete=false;
            }
            if (parm.startsWith("--search=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                exportNotes->query = parm;
            }
            if (parm.startsWith("--output=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                exportNotes->outputFile = parm;
            }
        }
        if (command->at(STARTUP_IMPORT)) {
            if (parm.startsWith("--input=", Qt::CaseSensitive)) {
                parm = parm.mid(8);
                importNotes->inputFile = parm;
            }
        }
        if (command->at(STARTUP_BACKUP)) {
            if (parm.startsWith("--output=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                exportNotes->outputFile = parm;
            }
        }
        if (command->at(STARTUP_READNOTE)) {
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                extractText->lid = parm.toInt();
            }
        }
        if (command->at(STARTUP_SIGNALGUI)) {
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                signalGui->lid = parm.toInt();
            }
            if (parm.startsWith("--show", Qt::CaseSensitive))
                signalGui->show=true;
            if (parm.startsWith("--synchronize", Qt::CaseSensitive))
                signalGui->synchronize=true;
            if (parm.startsWith("--screenshot", Qt::CaseSensitive))
                signalGui->takeScreenshot=true;
            if (parm.startsWith("--openNote", Qt::CaseSensitive))
                signalGui->openNote=true;
            if (parm.startsWith("--openExternalNote", Qt::CaseSensitive))
                signalGui->openExternalNote=true;
            if (parm.startsWith("--openNoteNewTab", Qt::CaseSensitive))
                signalGui->openNoteNewTab=true;
            if (parm.startsWith("--newNote", Qt::CaseSensitive))
                signalGui->newNote=true;
            if (parm.startsWith("--newExternalNote", Qt::CaseSensitive))
                signalGui->newExternalNote=true;
            if (parm.startsWith("--shutdown", Qt::CaseSensitive))
                signalGui->shutdown=true;
        }
        if (command->at(STARTUP_ALTERNOTE)) {
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                QRegExp regExp("[ ,;]");
                QStringList tokens = parm.split(regExp);
                for (int i=0; i<tokens.size(); i++) {
                    if (tokens[i].trimmed() != "")
                        alter->lids.append(tokens[i].toInt());
                }
            }
            if (parm.startsWith("--search=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                alter->query = parm;
            }
            if (parm.startsWith("--notebook=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                alter->notebook = parm;
            }
            if (parm.startsWith("--addTag=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                alter->addTagNames.append(parm);
            }
            if (parm.startsWith("--delTag=", Qt::CaseSensitive)) {
                parm = parm.mid(9);
                alter->delTagNames.append(parm);
            }
            if (parm == "--clearReminder") {
                alter->clearReminder=true;
            }
            if (parm == "--reminderComplete") {
                alter->reminderCompleted=true;
            }
        }
        if (command->at(STARTUP_EMAILNOTE)) {
            if (parm == "--ccSelf") {
                email->ccSelf = true;
            }
            if (parm.startsWith("--to=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                email->to = parm;
            }
            if (parm.startsWith("--cc=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                email->cc = parm;
            }
            if (parm.startsWith("--bcc=", Qt::CaseSensitive)) {
                parm = parm.mid(6);
                email->bcc = parm;
            }
            if (parm.startsWith("--note=", Qt::CaseSensitive)) {
                parm = parm.mid(7);
                email->note = parm;
            }
            if (parm.startsWith("--subject=", Qt::CaseSensitive)) {
                parm = parm.mid(10);
                email->subject = parm;
            }
            if (parm.startsWith("--id=", Qt::CaseSensitive)) {
                parm = parm.mid(5);
                email->lid = parm.toInt();
            }
        }
        if (command->at(STARTUP_OPENNOTEBOOK)) {
            if (parm.startsWith("--notebook=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                notebookList.append(parm);
            }
        }
        if (command->at(STARTUP_CLOSENOTEBOOK)) {
            if (parm.startsWith("--notebook=", Qt::CaseSensitive)) {
                parm = parm.mid(11);
                notebookList.append(parm);
            }
        }
        if (command->at(STARTUP_SQLEXEC)) {
            this->sqlExec=true;
            if (parm.startsWith("--query", Qt::CaseSensitive)) {
                parm = parm.mid(8);
            }
            if (!parm.startsWith("sqlExec", Qt::CaseInsensitive)) {
                sqlString = sqlString + " " + parm;
            }
        }
    }

    if ((!configDir.isEmpty()) && userDataDir.isEmpty()) {
        QLOG_WARN() << "As you provided configDir but not provided userDataDir, userDataDir will fallback to configDir: "
                       << configDir;
        userDataDir = configDir;
    }


    if (command->count(true) == 0)
        command->setBit(STARTUP_GUI,true);

    if (command->count(true) > 1) {
        QLOG_FATAL() << "\nInvalid options specified.  Only one command may be specified at a time.\n";
        return 16;
    }

    // Check for GUI overrides
    for (int i=0; i<argc; i++) {
        QString value = QString(argv[i]);
        if (value == "--forceNoGui") {
            guiAvailable = false;
            command->clearBit(STARTUP_GUI);
            i = argc;
        }
        if (value == "--forceGui") {
            guiAvailable = true;
            command->setBit(STARTUP_GUI);
            i = argc;
        }
    }

    return 0;
}

bool StartupConfig::query() {
    return command->at(STARTUP_QUERY);
}

bool StartupConfig::gui() {
     return command->at(STARTUP_GUI);
}

bool StartupConfig::sync() {
    return command->at(STARTUP_SYNC);
}

bool StartupConfig::addNote() {
    return command->at(STARTUP_ADDNOTE);
}

bool StartupConfig::appendNote() {
    return command->at(STARTUP_APPENDNOTE);
}

bool StartupConfig::show() {
    return command->at(STARTUP_SHOW);
}

bool StartupConfig::shutdown() {
    return command->at(STARTUP_SHUTDOWN);
}

bool StartupConfig::deleteNote() {
    return command->at(STARTUP_DELETENOTE);
}


bool StartupConfig::readNote() {
    return command->at(STARTUP_READNOTE);
}

bool StartupConfig::emailNote() {
    return command->at(STARTUP_EMAILNOTE);
}


bool StartupConfig::exports() {
    return command->at(STARTUP_EXPORT);
}

bool StartupConfig::import() {
    return command->at(STARTUP_IMPORT);
}

bool StartupConfig::backup() {
    return command->at(STARTUP_BACKUP);
}


bool StartupConfig::alterNote() {
    return command->at(STARTUP_ALTERNOTE);
}

bool StartupConfig::openNotebook() {
    return command->at(STARTUP_OPENNOTEBOOK);
}

bool StartupConfig::closeNotebook() {
    return command->at(STARTUP_CLOSENOTEBOOK);
}

bool StartupConfig::signalOtherGui() {
    return command->at(STARTUP_SIGNALGUI);
}

void StartupConfig::setAccountId(int accountId) {
    if (this->accountId == accountId) {
        return;
    }
    QLOG_DEBUG() << "StartupConfig: updating accountId to" << accountId;
    this->accountId = accountId;
}


