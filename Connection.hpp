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

//represents a connection the client
class Connection {
private:
    Socket m_socket;
public:
    Connection(int port, DataBase *data_base) : m_socket(port) {
        //receive transactions
        while (true) {
            Transaction transaction(&m_socket, data_base);
            while (true){
                int receive = transaction.receive();

                if (receive == -1) { //check if successful
                    return;
                }
                if (receive == -2) { //check if successful
                    break;
                }

            }

            std::cout << "finalized \n";
            transaction.commit();
        }
    }
};

#endif DOGEBASE_CONNECTION_HPP