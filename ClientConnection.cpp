/*
 * ClientConnection.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: tomer
 */

#include "ClientConnection.h"

ClientConnection::ClientConnection(tcp::socket *socket, Server *server,
		u_int32_t id) :
    id_(id), connection_(socket),active(false) {
	this->pServer_ = server;
	waitForPacket();
}

ClientConnection::~ClientConnection() {
	// TODO Auto-generated destructor stub
}

void ClientConnection::send(PacketForClient *packet) {
	bool isEmpty = this->outPacketsBuffer_.empty();
	this->outPacketsBuffer_.push_back(packet);
	if (isEmpty) {
		connection_.async_write(*(this->outPacketsBuffer_.front()),
				boost::bind(&ClientConnection::handleSendPacket, this,
						boost::asio::placeholders::error));
	}
}

void ClientConnection::waitForPacket() {
	std::cout << "waiting for packets from client\n";
	std::cout.flush();
	PacketForServer *newPacket = new PacketForServer();
	connection_.async_read(*newPacket,
			boost::bind(&ClientConnection::handleReceivePacket, this,
					boost::asio::placeholders::error, newPacket));
}

void ClientConnection::handleReceivePacket(const boost::system::error_code& e,
		PacketForServer *newPacket) {

	if (!e) {
		waitForPacket();
        switch (newPacket->opcode_) {
        case 1: //new event
            pServer_->newEventPrv(this->id_,*newPacket);
            break;
        default:
            break;
        }
		std::cout.flush();
	} else {
        std::cout << "error handleReceivePacket\n";
        std::cout << e.value();
        std::cout << e.category().name();
		pServer_->deleteConnection(id_);
	}
	delete newPacket;
}

void ClientConnection::handleSendPacket(const boost::system::error_code& e) {

	if (!e) {
		std::cout << "packet sent to client!\n";
		PacketForClient *tmp = this->outPacketsBuffer_.front();
		this->outPacketsBuffer_.pop_front();
		delete tmp;
		if (!(this->outPacketsBuffer_.empty())) {
			connection_.async_write(*(this->outPacketsBuffer_.front()),
					boost::bind(&ClientConnection::handleSendPacket, this,
							boost::asio::placeholders::error));
		}
	} else {
		std::cout << "error send to client\n";
		pServer_->deleteConnection(id_);
	}
}
