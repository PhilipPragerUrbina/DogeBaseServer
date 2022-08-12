//
// Created by Philip on 8/8/2022.
//

#ifndef DOGEBASE_TRANSACTION_HPP
#define DOGEBASE_TRANSACTION_HPP


#include <fstream>
#include "Socket.hpp"
#include "File.hpp"
class Transaction {
public:
    Socket* m_socket;
    bool m_complete = false;
    int op = 12;
    int id;
    DogeObject obj = DogeObject("a");
    File database{"database.dogetable"};
    File indexes{"index.dogetable"};
Transaction(Socket* socket){
    m_socket = socket;

    //one for index, one for object, one for operation
    for (int i = 0; i < 3; i++) {
        //get data
        std::string bytes_in = socket->read();
        //std::cout << bytes_in << " <- received string \n";
        //check data
        if (socket->isDisconnected()) {
            break;
        }
        if (bytes_in == "stop") {
            break;
        }
        if (bytes_in == "clear") {
           indexes.clear();
           database.clear();

            m_socket->write("k");
            m_complete = true;
            break;
        }

        switch (i) {
            case 0:
                op = DogeInt(bytes_in);
                m_socket->write("k");
                break;
            case 1:
                id = DogeInt(bytes_in);
                if (op == 0) {
                    m_complete = true;
                } else {
                    m_socket->write("k");
                }

                break;
            case 2:
                obj =  DogeObject(bytes_in);
                m_complete = true;

                break;


        }
        if(m_complete){
            break;
        }

    }
}

bool commit(){
    if (m_complete) {
        if (op == 0) {

            DogeInt index(indexes.readItem(0));
            if(id > -1 && id < index){
                std::cout << "Returned " << id << "\n";
                m_socket->write(database.readItem(id));
            }else{
                std::cout << "error " << id << "\n";
                m_socket->write("error");
            }

        } else if(op == 1){
            //write
              database.writeItem(&obj);
            DogeInt index(indexes.readItem(0));

            index = index + 1;
            indexes.overWriteItem(&index);
            index = index - 1;
            std::cout << "Added id: " << index << " \n";
            m_socket->write(&index);
        }
    }
    return m_complete;

}
};

#endif DOGEBASE_TRANSACTION_HPP