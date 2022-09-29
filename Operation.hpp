//
// Created by Philip on 8/12/2022.
//

#pragma once

#include "TransactionMemory.hpp"
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
    virtual void receive(Socket *socket, DataBase *data_base, TransactionMemory *memory) {};
    //do atomic action once everything else is successful
    virtual void finalize(DataBase* data_base){};
};



//stop the connection
class StopOperation : public Operation{
public:
    void receive(Socket *socket, DataBase *data_base, TransactionMemory *memory) {
        std::cout << "Request to Stop \n";
        throw DogeException("Client Stop Message");
    }
    void finalize(DataBase *data_base) {}
};
//clear the database
class ClearOperation : public Operation{
public:
    void receive(Socket *socket, DataBase *data_base, TransactionMemory *memory) {
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
    void receive(Socket *socket, DataBase *data_base, TransactionMemory *memory) {


        m_id = DogeInt(socket->read());

        //todo get num attibutes return document
        std::cout << "Request to read:" << m_id << " \n";
        if (socket->isDisconnected()) {
            throw DogeException("Disconnected from Client");
        }
        DogeType *memory_data = memory->getMemory(m_id);
        if (memory_data == nullptr) {
            DogeInt index(data_base->m_metadata.readItem(0));
            if (m_id > -1 && m_id < index) {


                socket->write(data_base->m_data.readItem(m_id), 0);


            } else {
                throw DogeException("Bounds error");
            }
        } else {
            socket->write(memory_data->serialize(), 0);
        }
        std::cout << "Read fulfilled \n";
    }
};

//push back to database
class AppendOperation : public Operation{
private:
    //data
    std::vector<DogeType> m_objects{""};
    std::vector<DogeInt> m_ids = 0;
public:
    void receive(Socket *socket, DataBase *data_base, TransactionMemory *memory) {


        //output the request type
        std::cout << "Request to write \n";

        //get then length of the request(# of items in document)
        int length = DogeInt(socket.read("Length read"));
        for (int i = 0; i < length; ++i) {
            //recieve type
            DogeTypeID t = DogeInt(socket.read("type read"));
            //receive object
            DogeType object = DogeDogeTypeObject(socket->read()); //todo  make it be correct type
            int id = DogeInt(data_base->m_metadata.readItem(0));
            id = id + memory->getIDOffset(); //add temporary offset(we dont want to incrment the global latest id until finalized)
            memory->incrementID(); //increment offset
            memory->setMemory(id, &object);
            m_objects.push_back(object);
            m_ids.push_back(id);
        }




        socket->write(&m_id);
    }
    void finalize(DataBase *data_base) {
        data_base->m_data.writeItem(&m_object); //write object
        int latest_id  = DogeInt(data_base->m_metadata.readItem(0));
        if(latest_id < m_id){
            m_id = m_id + 1;
            data_base->m_metadata.overWriteItem(&m_id); //write the latest id
        }
        std::cout << "Write fulfilled at id: " << m_id << " \n";
    }
};
