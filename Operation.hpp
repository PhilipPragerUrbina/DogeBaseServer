//
// Created by Philip on 8/12/2022.
//

#ifndef DOGEBASE_OPERATION_HPP
#define DOGEBASE_OPERATION_HPP

#include "Socket.hpp"
#include "DataBase.hpp"
#include "DogeException.hpp"

//Doge opcodes
enum OpCode{
    DOGE_WRITE,DOGE_READ, DOGE_APPEND, DOGE_STOP, DOGE_CLEAR,DOGE_DELETE
};

//operation virtual class
class Operation {
public:
    //do initial action
    virtual void receive(Socket* socket, DataBase *data_base){};
    //do atomic action once everything else is successful
    virtual void finalize(DataBase* data_base){};
};

//stop the connection
class StopOperation : public Operation{
public:
    void receive(Socket *socket, DataBase *data_base) {
        std::cout << "Request to Stop \n";
        throw DogeException("Client Stop Message");
    }
    void finalize(DataBase *data_base) {}
};
//clear the database
class ClearOperation : public Operation{
public:
    void receive(Socket *socket, DataBase *data_base) {
        std::cout << "Request to clear \n";
    }
    void finalize(DataBase *data_base) {
        std::cout << "Clear fulfilled \n";
        data_base->m_metadata.clear();
        data_base->m_data.clear();
    }
};

// read an item from the database
class ReadOperation : public Operation{
private:
    DogeInt m_id = 0;
public:
    void receive(Socket *socket, DataBase *data_base) {

        m_id = DogeInt(socket->read());
        std::cout << "Request to read:" << m_id << " \n";
        if (socket->isDisconnected()) {
            throw DogeException("Disconnected from Client");
        }
        DogeInt index(data_base->m_metadata.readItem(0));
        if(m_id > -1 && m_id < index){
            socket->write(data_base->m_data.readItem(m_id),0);
        }else{
            throw DogeException("Bounds error");
        }
        std::cout << "Read fulfilled \n";
    }
};
//push back to database
class AppendOperation : public Operation{
private:
    //data
    DogeObject m_object{""};
    DogeInt m_id = 0;
public:
    void receive(Socket *socket, DataBase *data_base) {
        std::cout << "Request to write \n";
        m_object = DogeObject(socket->read());
        if (socket->isDisconnected()) {
            throw DogeException("Disconnected from Client");
        }
        m_id = DogeInt(data_base->m_metadata.readItem(0));
        socket->write(&m_id);
    }
    void finalize(DataBase *data_base) {
        data_base->m_data.writeItem(&m_object);
        m_id = m_id +  1;
        data_base->m_metadata.overWriteItem(&m_id);
        std::cout << "Write fulfilled at id: " << m_id << " \n";
    }
};
#endif DOGEBASE_OPERATION_HPP