#ifndef QTSERVER_H
#define QTSERVER_H

#include <QObject>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "/home/tomer/workspace/Server/src/Server.h"

class Server;
class QtServer : public QObject
{
    Q_OBJECT
public:
    explicit QtServer(QObject *parent = 0);
    virtual ~QtServer();

signals:
    void newEvent();
    void newConnection(size_t id);
    
public slots:
//    void receiveEvent();
private:
    boost::asio::io_service io_service_;
    Server *server_;
    boost::thread *runThread_;
};

#endif // QTSERVER_H
