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

//class to represent a transaction for atomicity
class Transaction {
private:
    //info needed to do operations
    Socket* m_socket;
    DataBase* m_data_base;
    //operations in transaction
   std::vector<Operation*> m_operations;
public:

    //set info
    Transaction(Socket* socket, DataBase* data_base){
    m_data_base = data_base;
    m_socket = socket;
    }

    //receive an operation through the socket and add to transactions
    int receive(){

        //read and respond
        OpCode opcode = OpCode(DogeInt(m_socket->read("opcode received")).getValue());
        if (m_socket->isDisconnected()) { //error check
            return -1;
        }

        //create corresponding operation
        Operation* operation;
        switch (opcode) {
            case DOGE_STOP:
                operation = new StopOperation();
                return -2;
            case DOGE_READ:
                operation = new ReadOperation();
                break;
            case DOGE_APPEND:
                operation = new AppendOperation();
                break;
            case DOGE_CLEAR:
                operation = new ClearOperation();
                break;
            case DOGE_DELETE:
                //dont delete, just overwrite with deleteion
                //compress m_data later
                break;
        }
        if(operation == nullptr){
            return -1;
        }
        //add operation
        m_operations.push_back(operation);

        //do initial action
        return operation->receive(m_socket,m_data_base);
    }
    //finalize and apply all operations
void commit(){
    for (Operation* op : m_operations) {
        if(op != nullptr) {
            op->finalize(m_data_base);
        }
    }

}
//clean up
~Transaction(){
    for (Operation* op : m_operations) {
        if(op != nullptr) {
           delete op;
        }
    }

    }
};

#endif DOGEBASE_TRANSACTION_HPP