//
// Created by Philip on 8/8/2022.
//

#ifndef DOGEBASE_TRANSACTION_HPP
#define DOGEBASE_TRANSACTION_HPP


#include <fstream>
#include "Socket.hpp"
#include "DogeType.hpp"

class Transaction {
public:
    Socket* m_socket;
    bool m_complete = false;
Transaction(Socket* socket){
    m_socket = socket;
    int op;
    int id;
    DogeObject *obj;
    //one for index, one for object, one for operation
    for (int i = 0; i < 3; i++) {
        //get data
        std::string bytes_in = socket->read();
        std::cout << bytes_in << " <- received string \n";
        //check data
        if (socket->isDisconnected()) {
            break;
        }
        if (bytes_in == "stop") {
            break;
        }
        if (bytes_in == "clear") {
            std::ofstream clear ("index.dogetable");
            clear.clear();
            clear.close();
            std::ofstream c ("database.dogetable");
            c .clear();
            c.close();

            m_socket->write("k");
            m_complete = true;
            break;
        }

        switch (i) {
            case 0:
                op = DogeInt(bytes_in).getValue();
                m_socket->write("k");
                break;
            case 1:
                id = DogeInt(bytes_in).getValue();
                if (op == 0) {
                    m_complete = true;
                } else {
                    m_socket->write("k");
                }

                break;
            case 2:
                obj = new DogeObject(bytes_in);
                m_complete = true;

                break;


        }
        if (m_complete) {
            if (op == 0) {
                std::ifstream infile ("database.dogetable");
                std::cout << "Returned " << id << "\n";

                std::string str;


                for (int i = 0; i < id+1; ++i) {

                    char data [4];

                    if(!infile.read( data, 4)){
                        break;
                    };
                    int size = DogeInt(data).getValue();
                    //  std::cout << size << " size \n";
                    char data_2 [size];

                    if(!infile.read(data_2,size)){
                        break;
                    };

                    str = std::string (data_2,size);


                }



                std::cout << str << "\n";
                //read
                m_socket->write(str);
                infile.close();
                break;
            } else {
                std::ofstream outfile ("database.dogetable",std::ofstream::app);
                std::cout << "Added \n";
                //write
                outfile  <<DogeInt(obj->serialize().size()).serialize() << obj->serialize();


                outfile.close();
                std::ifstream infile ("index.dogetable");
                std::string str = "    ";
                bool read = false;
                int num = 0;
                while (   infile.read( &*str.begin(), 4)) {
                    num++;
                    read = true;

                }
                std::cout << "num: " << num << "\n";

                infile.close();
                DogeInt index(str);
                str.clear();
                if(!read){
                    index = -1;
                }



                std::ofstream outfile_2 ("index.dogetable",std::ofstream::app);
                outfile_2.write(DogeInt(index.getValue() + 1).serialize().data(), 4 );
                outfile_2.close();
                std::cout << "id: " << index.getValue() + 1 << " \n";
                m_socket->write(DogeInt(index.getValue() + 1).serialize());


            }
        }


    }
}
};

#endif DOGEBASE_TRANSACTION_HPP