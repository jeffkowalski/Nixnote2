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

#include "accountsmanager.h"
#include "src/global.h"

extern Global global;


//********************************************
//* Constructor
//********************************************
AccountsManager::AccountsManager(int id, QObject *parent) :
    QObject(parent) {
    currentId = id;
    configFile = global.fileManager.getConfigDir() + NN_ACCOUNTS_CONFIG_FILE_PREFIX + ".conf";
    if (!QFile(configFile).exists()) {
        QFile xmlFile(configFile);
        xmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QXmlStreamWriter writer(&xmlFile);
        writer.setAutoFormatting(true);
        writer.setCodec("UTF-8");
        writer.writeStartDocument();
        writer.writeDTD("<!DOCTYPE NixNote-Accounts>");
        writer.writeStartElement("user-accounts");
        writer.writeStartElement("account");
        writer.writeTextElement("name", "Default Account");
        writer.writeTextElement("id", "1");
        writer.writeEndElement();
        writer.writeEndElement();
        writer.writeEndDocument();
        xmlFile.close();
    }

    QFile file(configFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    doc.setContent(&file);
    file.close();
    QDomNodeList nodes = doc.elementsByTagName("account");
    for (int i = 0; i < nodes.size(); i++) {
        QDomElement element = nodes.at(i).toElement();
        QDomNode idNode = element.firstChildElement("id");
        int value = idNode.toElement().text().toInt();
        if (value == currentId) {
            currentNode = nodes.at(i);
            i = nodes.size();
        }
    }
}


//********************************************
//* Do we have a good oauth token?
//********************************************
bool AccountsManager::oauthTokenFound() {

    if (this->getOAuthToken().length() > 0)
        return true;
    else
        return false;
}


//********************************************
//* Get a list of account IDs
//********************************************
QList<int> AccountsManager::idList() {
    QList<int> ids;
    QDomNodeList nodes = doc.elementsByTagName("account");
    for (int i = 0; i < nodes.size(); i++) {
        QDomElement element = nodes.at(i).toElement();
        QDomNode idNode = element.firstChildElement("id");
        int value = idNode.toElement().text().toInt();
        ids.append(value);
    }
    return ids;
}


//********************************************
//* get a list of defined account names
//********************************************
QStringList AccountsManager::nameList() {
    QStringList names;
    QDomNodeList nodes = doc.elementsByTagName("account");
    for (int i = 0; i < nodes.size(); i++) {
        QDomElement element = nodes.at(i).toElement();
        QDomNode idNode = element.firstChildElement("name");
        QString value = idNode.toElement().text();
        names.append(value);
    }
    return names;
}


//********************************************
//* Get an oauth token for the account
//* currently in use
//********************************************
QString AccountsManager::getOAuthToken() {
    QDomElement element = currentNode.toElement();
    QDomNode consumerNode = element.firstChildElement("consumerKey");
    if (consumerNode.toElement().text() != "baumgarr-3523")   // Current EDAM_CONSUMER_KEY
        return "";
    QDomNode tokenNode = element.firstChildElement("oauth-token");
    return tokenNode.toElement().text();
}


//********************************************
//* Set the oauth token for the current
//* account.
//********************************************
void AccountsManager::setOAuthToken(QString token) {
    QString server = this->getServer();
    QString name = this->getName();

    this->removeId(currentId);
    this->addId(currentId, name, token, server);
    QDomNodeList nodes = doc.elementsByTagName("account");
    for (int i = 0; i < nodes.size(); i++) {
        QDomElement element = nodes.at(i).toElement();
        QDomNode idNode = element.firstChildElement("id");
        int value = idNode.toElement().text().toInt();
        if (value == currentId) {
            currentNode = nodes.at(i);
            i = nodes.size();
        }
    }
}


//********************************************
//* Change the name for an account
//********************************************
void AccountsManager::setName(QString name, int id) {
    QString server = this->getServer();
    QString token = this->getOAuthToken();
    if (id <= 0)
        id = currentId;

    this->removeId(id);
    this->addId(id, name, token, server);
}


//********************************************
//* Set the server for an acocunt
//********************************************
QString AccountsManager::getServer() {
    QDomElement element = currentNode.toElement();
    QDomNode tokenNode = element.firstChildElement("server");
    QString server = tokenNode.toElement().text();
    if (server == "")
        server = "www.evernote.com";
    return server;
}


//********************************************
//* Get the name of the account in use
//********************************************
QString AccountsManager::getName() {
    QDomElement element = currentNode.toElement();
    QDomNode tokenNode = element.firstChildElement("name");
    QString server = tokenNode.toElement().text();
    return server;
}


//********************************************
//* Add an account
//********************************************
int AccountsManager::addId(int id, QString name, QString oauth, QString server) {
    if (id <= 0)
        id = getNewIdNumber();

    QDomElement account = doc.createElement("account");

    QDomElement idElement = doc.createElement("id");
    QDomText idText = doc.createTextNode(QString::number(id));
    idElement.appendChild(idText);
    account.appendChild(idElement);

    QDomElement nameElement = doc.createElement("name");
    QDomText nameText = doc.createTextNode(name);
    nameElement.appendChild(nameText);
    account.appendChild(nameElement);

    if (oauth.trimmed() != "") {
        QDomElement consumerKeyElement = doc.createElement("consumerKey");
        QDomText consumerKeyText = doc.createTextNode("baumgarr-3523");   // current EDAM_CONSUMER_KEY = "baumgarr-3523"
        consumerKeyElement.appendChild(consumerKeyText);
        account.appendChild(consumerKeyElement);

        QDomElement oauthElement = doc.createElement("oauth-token");
        QDomText oauthText = doc.createTextNode(oauth);
        oauthElement.appendChild(oauthText);
        account.appendChild(oauthElement);
    }

    if (server.trimmed() != "" && server != "www.evernote.com") {
        QDomElement serverElement = doc.createElement("server");
        QDomText serverText = doc.createTextNode(server);
        serverElement.appendChild(serverText);
        account.appendChild(serverElement);
    }

    doc.documentElement().appendChild(account);
    save();
    return id;
}


//********************************************
//* save to accounts.conf
//********************************************
void AccountsManager::save() {
    QFile xmlFile(configFile);
    xmlFile.open(QIODevice::WriteOnly | QIODevice::Text);
    xmlFile.write(doc.toByteArray());
    xmlFile.close();
}


//********************************************
//* Remove an account
//********************************************
bool AccountsManager::removeId(int id) {
    QDomNodeList nodes = doc.elementsByTagName("account");
    for (int i = 0; i < nodes.size(); i++) {
        QDomElement element = nodes.at(i).toElement();
        QDomNode idNode = element.firstChildElement("id");
        int value = idNode.toElement().text().toInt();
        if (value == id) {
            nodes.at(i).parentNode().removeChild(nodes.at(i));
            save();
            return true;
        }
    }
    return false;
}


//********************************************
//* Increment the account to the next ID number
//* available.
//********************************************
int AccountsManager::getNewIdNumber() {
    QList<int> numbers = idList();
    for (int i = 1;; i++)
        if (!numbers.contains(i))
            return i;
}


//********************************************
//* set the oauth token for a specific
//* account.
//********************************************
void AccountsManager::setOAuthToken(int id, QString token) {
    AccountsManager manager(id);
    manager.setOAuthToken(token);
}
