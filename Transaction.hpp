//
// Created by Philip on 8/8/2022.
//

#ifndef DOGEBASE_TRANSACTION_HPP
#define DOGEBASE_TRANSACTION_HPP


#include <fstream>
#include "Socket.hpp"
#include "File.hpp"
#include "DataBase.hpp"
#include "Operation.hpp"
class Transaction {
private:
    Socket* m_socket;
    DataBase* m_data_base;
   Operation* m_operation = nullptr;
public:
    Transaction(Socket* socket, DataBase* data_base){
    m_data_base = data_base;
    m_socket = socket;
    }

    int receive(){
        OpCode opcode = OpCode(DogeInt(m_socket->read("opcode received")).getValue());
        if (m_socket->isDisconnected()) {
            return -1;
        }
        switch (opcode) {
            case DOGE_STOP:
                m_operation = new StopOperation();
                return -1;
            case DOGE_READ:
                m_operation = new ReadOperation();
                break;
            case DOGE_APPEND:
                m_operation = new AppendOperation();
                break;
            case DOGE_CLEAR:
                m_operation = new ClearOperation();
                break;
            case DOGE_DELETE:
                //dont delete, just overwrite with deleteion
                //compress m_data later
                break;
        }
        return m_operation->receive(m_socket,m_data_base);
    }

void commit(){
    if(m_operation != nullptr) {
        m_operation->finalize(m_data_base);
    }
}
~Transaction(){
        if(m_operation != nullptr){
            delete m_operation;
        }

    }
};

#endif DOGEBASE_TRANSACTION_HPP