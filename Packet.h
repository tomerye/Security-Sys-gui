/*
 * Packet.h
 *
 *  Created on: Apr 6, 2013
 *      Author: tomer
 */

#ifndef PACKET_H_
#define PACKET_H_



struct Packet {
    int id_;
    int opcode_;
    std::string file_path_;
    std::string time_;
    int priority_;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & opcode_;
        ar & file_path_;
        ar & id_;
        ar & time_;
        ar & priority_;
    }
};

#endif /* PACKET_H_ */
