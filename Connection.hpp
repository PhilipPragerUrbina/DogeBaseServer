//
// Created by Philip on 8/8/2022.
//

#ifndef DOGEBASE_CONNECTION_HPP
#define DOGEBASE_CONNECTION_HPP
#include <vector>
#include <fstream>
#include "Socket.hpp"
#include "DogeType.hpp"
#include "Transaction.hpp"

class Connection {
public:
    Socket m_socket;
Connection(int port) : m_socket(port){
    while(true) {
 Transaction transaction(&m_socket);
     if(!transaction.m_complete){
         break;
     }
    }
}
};

#endif DOGEBASE_CONNECTION_HPP