/*
 * Server.h
 *
 *  Created on: Apr 6, 2013
 *      Author: tomer
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <QObject>
#include <boost/asio.hpp>
#include <iostream>
#include "ClientConnection.h"
#include <boost/bind.hpp>
#include <QVector>
#include <QString>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QDebug>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>

//#include "FtpClient/FTPClient.h"

#define PIC_FOLDER "/home/tomer/securitySystemPic/"

class ClientConnection;
using boost::asio::ip::tcp;
class Server :public QObject{
   Q_OBJECT
public:
	Server(int port, boost::asio::io_service &io_service);
	virtual ~Server();
	void deleteConnection(const u_int32_t id);
	void send(const u_int32_t id, PacketForClient *packet);
    void newEventPrv(u_int32_t clientid ,PacketForServer event);
    void getFile(u_int32_t clientid, std::string srcPath);
    void setClientState(u_int32_t clientid, bool state);
    bool getClientState(u_int32_t clientid);

private:
	tcp::endpoint endpoint_;
	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
	std::map<u_int32_t, ClientConnection*> connection_map_;
    QNetworkAccessManager downloadManager;
    QNetworkReply *reply;
    QFile savePic;

	void startAccept();
	void addNewConnection(u_int32_t *id, tcp::socket *newSocket,
			const boost::system::error_code& e);
	void handleGetNewConnectionID(const boost::system::error_code& e,
			tcp::socket *newSocket);


signals:
    void newEvent(QVector<QString>);
    void newPicture(QString);
    void newConnection(u_int32_t);
    void deleteConnectionSignal(u_int32_t);
    void downloadProgressSignal(qint64,qint64);

public slots:
    void newFtpPic();
    void downloadProgressSlot(qint64,qint64);

};

#endif /* SERVER_H_ */
