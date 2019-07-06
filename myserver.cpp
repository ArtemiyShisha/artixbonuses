#include "myserver.h"

myserver::myserver(){}

myserver::~myserver(){}

void myserver::startServer() {
    if (this -> listen(QHostAddress::Any, 5555)) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("D:\\bonuses.db");
        if (db.open()) {
            qDebug() << "Listening and DB is opened";
        } else {
            qDebug() << "DB`s not opened";
        }

    } else {
        qDebug() << "Not listening";
    }
}

void myserver::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket(this);
    socket -> setSocketDescriptor(socketDescriptor);

        connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

    qDebug() << socketDescriptor << " Client connected";

    socket -> write("{\"type\":\"connect\",\"status\":\"yes\"}");
    qDebug() << "Send a client connection status - YES";
}

void myserver::sockReady() {
    Data = socket -> readAll();
    qDebug() << "Data: " << Data;

    doc = QJsonDocument::fromJson(Data, &docError);

    if (docError.errorString() == "no error occurred") {
        if ((doc.object().value("type").toString() == "select") && (doc.object().value("params").toString() == "bonuses")) {
            if (db.isOpen()) {
                QByteArray total = "{\"type\":\"resultSelect\",\"result\":[";

                QSqlQuery* query = new QSqlQuery(db);
                if (query -> exec("SELECT bonuses FROM bonuseslist")) {
                    while (query -> next()) {
                        total.append("{\"bonuses\":\"" + query -> value(0).toString() + "\"},");
                    }

                    total.remove(total.length()-1,1);
                    total.append("]}");

                    socket -> write(total);
                    socket -> waitForBytesWritten(500);

                } else {
                    qDebug() << "Query`s not succes";
                }
            }
        }
    }
}

void myserver::sockDisc() {
    qDebug() << "Disconnect";
    socket -> deleteLater();
}
