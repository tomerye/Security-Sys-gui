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
#include "Packet.h"
#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include "Server.h"

class Server;
using boost::asio::ip::tcp;

class ClientConnection {
public:
	ClientConnection(tcp::socket *socket, Server *server, size_t id);
	virtual ~ClientConnection();
	tcp::socket& getSocket() const;
	void send(Packet Packet);
    std::string getSite(){return site_;}
    std::string getLocation(){return location_;}
    tcp::socket* getSocket(){return connection_.getSocket();}

private:
//	std::vector<Packet> packetsVec;
	Server *pServer_;
	size_t id_;
    std::string site_;
    std::string location_;
	void waitForPacket();
	void handleReceivePacket(const boost::system::error_code& e,
			std::vector<Packet> *packetsVec);
	void sendResult(const boost::system::error_code& e,
			std::vector<Packet> *packetsVec);
	AsyncSerializationConnection connection_;
};

#endif /* CLIENTCONNECTION_H_ */
