//
// Created by Philip on 8/12/2022.
//

#ifndef DOGEBASE_OPERATION_HPP
#define DOGEBASE_OPERATION_HPP

#include "Socket.hpp"
#include "DataBase.hpp"

enum OpCode{
    DOGE_WRITE,DOGE_READ, DOGE_APPEND, DOGE_STOP, DOGE_CLEAR,DOGE_DELETE
};
class Operation {
public:
    virtual int receive(Socket* socket, DataBase *data_base){};
    virtual void finalize(DataBase* data_base){};
};
class StopOperation : public Operation{
public:
    int receive(Socket *socket, DataBase *data_base) {
        std::cout << "Request to Stop \n";
        return 1;
    }
    void finalize(DataBase *data_base) {}
};
class ClearOperation : public Operation{
public:
    int receive(Socket *socket, DataBase *data_base) {
        std::cout << "Request to clear \n";
        return 1;
    }
    void finalize(DataBase *data_base) {
        std::cout << "Clear fulfilled \n";
        data_base->m_metadata.clear();
        data_base->m_data.clear();
    }
};
class ReadOperation : public Operation{
private:
    DogeInt m_id = 0;
public:
    int receive(Socket *socket, DataBase *data_base) {
        m_id = DogeInt(socket->read());
        if (socket->isDisconnected()) {return -1;}
        std::cout << "Request to read:" << m_id << " \n";
        DogeInt index(data_base->m_metadata.readItem(0));
        if(m_id > -1 && m_id < index){
            socket->write(data_base->m_data.readItem(m_id));
        }else{
            std::cerr << "Read bounds error \n";
            socket->write("bounds error");
            return -2;
        }
        std::cout << "Read fulfilled \n";
        return 1;

    }
    void finalize(DataBase *data_base) {}
};
class AppendOperation : public Operation{
private:
    DogeObject m_object{""};
    DogeInt m_id = 0;
public:
    int receive(Socket *socket, DataBase *data_base) {
        m_object = DogeObject(socket->read());
        if (socket->isDisconnected()) {return -1;}
        std::cout << "Request to write \n";
        m_id = DogeInt(data_base->m_metadata.readItem(0));
        socket->write(&m_id);
        return 1;

    }
    void finalize(DataBase *data_base) {
        data_base->m_data.writeItem(&m_object);
        m_id = m_id +  1;
        data_base->m_metadata.overWriteItem(&m_id);
        std::cout << "Write fulfilled at id: " << m_id << " \n";
    }
};
#endif DOGEBASE_OPERATION_HPP