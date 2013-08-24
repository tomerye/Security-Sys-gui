/*
 * ClientConnection.h
 *
 *  Created on: Apr 6, 2013
 *      Author: tomer
 */

#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include <boost/asio.hpp>
#include "AsyncSerializationConnection.cpp"
#include "PacketForClient.h"
#include "PacketForServer.h"
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "Server.h"

class Server;
using boost::asio::ip::tcp;

class ClientConnection {
public:
	ClientConnection(tcp::socket *socket, Server *server, u_int32_t id);
	virtual ~ClientConnection();
	tcp::socket& getSocket() const;
	void send(PacketForClient *packet);
    tcp::socket* getSocket(){return connection_.getSocket();}
    bool getState(){return active;}
    void setState(bool state){active=state;}

private:
	std::deque<PacketForClient*> outPacketsBuffer_;
	Server *pServer_;
	u_int32_t id_;
    bool active;
	void waitForPacket();
	void handleReceivePacket(const boost::system::error_code& e,
			PacketForServer *newPacket);
	void handleSendPacket(const boost::system::error_code& e);
	AsyncSerializationConnection connection_;
};

#endif /* CLIENTCONNECTION_H_ */
