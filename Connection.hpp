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
Connection(int port, DataBase* data_base) : m_socket(port){
    while(true) {
 Transaction transaction(&m_socket,data_base);
     if(transaction.receive() < 1){
         break;
     }

        transaction.commit();
    }
}
};

#endif DOGEBASE_CONNECTION_HPP