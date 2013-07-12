/*
 * Server.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: tomer
 */

#include "Server.h"

using namespace std;

Server::Server(int port, boost::asio::io_service &io_service) :
		endpoint_(tcp::v4(), port), io_service_(io_service), acceptor_(
				io_service, endpoint_) {
	startAccept();

}

void Server::startAccept() {
	cout << "Start accept!\n";
	tcp::socket *newSocket = new tcp::socket(io_service_);
	acceptor_.async_accept(*newSocket,
			boost::bind(&Server::handleGetNewConnectionID, this,
					boost::asio::placeholders::error, newSocket));
}

Server::~Server() {
// TODO Auto-generated destructor stub
}

void Server::handleGetNewConnectionID(const boost::system::error_code& e,
		tcp::socket *newSocket) {

	if (!e) {
		u_int32_t *clientId = new u_int32_t();
		boost::asio::async_read(*newSocket,
				boost::asio::buffer(clientId, sizeof(u_int32_t)),
				boost::bind(&Server::addNewConnection, this, clientId,
						newSocket, boost::asio::placeholders::error));
		startAccept();
	} else {

		std::cout << "error " << __FILE__ << __LINE__ << std::endl;
		delete newSocket;

	}

}

void Server::addNewConnection(u_int32_t *id, tcp::socket *newSocket,
		const boost::system::error_code& e) {
	std::cout << "received new ID:" << (u_int32_t) *id << std::endl;
	std::cout.flush();
	if (!e) {
		if (connection_map_.find(*id) == connection_map_.end()) {
			connection_map_[*id] = new ClientConnection(newSocket, this, *id);
            emit newConnection(*id);
		} else {
			std::cout << "someone with same id trying to connect!\n";
			delete newSocket;
			delete id;

		}
	} else {
		std::cout << "error addNewConnection/n";
		delete newSocket;
		delete id;
	}
}

void Server::deleteConnection(const u_int32_t id) {
	std::map<u_int32_t, ClientConnection*>::iterator it = connection_map_.find(
			id);
	cout << "Delete connection with ID:" << id << endl;
	if (it != connection_map_.end()) {
		connection_map_.erase(it);
        emit deleteConnectionSignal(id);
		std::cout << "id" << id << "deleted!/n";
	} else {
		std::cout << "no such id/n";
	}
}

void Server::send(const u_int32_t id, PacketForClient *packet) {
	std::map<u_int32_t, ClientConnection*>::iterator it = connection_map_.find(
			id);
	cout << "trying to send packet to ID:" << id << endl;
	if (it != connection_map_.end()) {
		it->second->send(packet);
	} else {
		std::cout << "no such id/n";
	}

}

void Server::newEventPrv(u_int32_t clientid,PacketForServer packet){
//    std::map<u_int32_t, ClientConnection*>::iterator it = connection_map_.find(clientid);

    QVector<QString> eventVector;
    std::stringstream ss;
    ss<<clientid;
    std::string strId;
    ss >> strId;
    eventVector << QString::fromStdString(strId) << QString::fromStdString(packet.time_) << QString::number(packet.priority_) << QString::fromStdString(packet.file_path_) ;
    emit newEvent(eventVector);
}

void Server::getFile(u_int32_t clientid, string srcPath){
    std::map<u_int32_t, ClientConnection*>::iterator it = connection_map_.find(clientid);
    if (it == connection_map_.end()) {
        qDebug() << "no such id/n";
        return;
    }
    boost::asio::ip::tcp::endpoint remote_ep = (*(it->second->getSocket())).remote_endpoint();
    boost::asio::ip::address remote_ad = remote_ep.address();
    std::string clientIP = remote_ad.to_string();
    qDebug() << QString::fromStdString(clientIP);
    QString url="ftp://";
    url+=QString::fromStdString(clientIP);
    url+="/";
    url+=QString::fromStdString(srcPath);
    qDebug() << url;
    QUrl ftpClientUrl(url);

    QNetworkRequest downloadReq(ftpClientUrl);
    this->reply = this->downloadManager.get(downloadReq);

//    connect(currentDownload,SIGNAL(downloadProgress(qint64,qint64)),)
    connect(reply,SIGNAL(finished()),this,SLOT(newFtpPic()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgressSlot(qint64,qint64)));
}

void Server::newFtpPic(){
    qDebug() << reply->error();

    qDebug() << reply->url();
    qDebug() << reply->rawHeader("Content-Length");
    QUrl receivedPicUrl = reply->url();
    QString path = receivedPicUrl.path();
    QString filename = QFileInfo(path).fileName();
    QString saveFilePath = PIC_FOLDER;
    saveFilePath += filename;
    QFile file(saveFilePath);
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();    
    reply->deleteLater();
    emit newPicture(saveFilePath);
    emit downloadProgressSignal(0);
}


void Server::downloadProgressSlot(qint64 received,qint64 total){
    if(total!=0)
        emit downloadProgressSignal((int)(received/total));
    else
        emit downloadProgressSignal(0);
}

