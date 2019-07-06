#ifndef MYSERVER_H
#define MYSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

class myserver: public QTcpServer {
   Q_OBJECT
public:
    myserver();
    ~myserver();

    QTcpSocket* socket; // Переменная, отвечающая за сокеты
    QByteArray Data; // Переменная для складирования и передачи данных по сети

    QJsonDocument doc;
    QJsonParseError docError;

    QSqlDatabase db;

public slots:
    void startServer(); // Запуск сервера
    void incomingConnection(qintptr socketDescriptor); // Описание действий при подключении клиента
    void sockReady(); // Готовность сокета
    void sockDisc(); // Отклбчение клиента от сокета
};

#endif // MYSERVER_H
