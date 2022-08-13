//
// Created by Philip on 8/1/2022.
//

#ifndef DOGEBASE_SOCKET_HPP
#define DOGEBASE_SOCKET_HPP


#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "DogeType.hpp"


class Socket {
private:
    sockaddr_in m_address;
    int m_client_socket;
    int m_server_socket;
    int m_port;
    int m_options = 1;
    bool m_disconnected = true;
public:
    Socket(int port = 8080){
        //get parameters
        m_port = port;
        std::cout << "Socket Created on port: " << m_port << "\n";

        //create server socket
        //INET since this connects to other machines.
        m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_server_socket == -1) { std::cerr << "Socket creation failed;"; return;}

        //set socket options
        //SO_KEEPALIVE
        if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &m_options, sizeof(m_options)) == -1) {
            std::cerr << "Socket m_options failed;";return;
        }

        //set address
        m_address.sin_family = AF_INET;
        m_address.sin_addr.s_addr = INADDR_ANY;
        m_address.sin_port = htons(m_port);

        //bind to address
        if (bind(m_server_socket, (struct sockaddr*)&m_address, sizeof(m_address)) == -1) {
            std::cerr << "Socket bind failed to " << m_port; return;
        }
        std::cout << "Socket waiting... \n";
        //listen for connections
        if (listen(m_server_socket, 3) == -1) {std::cerr << "Socket listen failed ";return;}

        //accept the connection and get the client socket
        int address_length = sizeof(m_address);
        m_client_socket = accept(m_server_socket, (sockaddr*)&m_address, (socklen_t*)&address_length);
        if (m_client_socket == -1) {std::cerr << "Socket accept failed ";return;
        }
        m_disconnected = false;
        std::cout << "Socket listening... \n";
    }

    //get data
    const std::string read(){
        if(m_disconnected){
            return "";
        }
        char buffer[1024] = {0};
        int number_read = ::read(m_client_socket, buffer, 1024) ;
        if(number_read <= 0){
            stop();
            return "";
        }

        return std::string (buffer, number_read);
    }

    //get data with response
    const std::string read(std::string response){
       std::string to_return = read();
        write(response);
        return to_return;
    }


    //write data
    const void write(const std::string message){
        if(m_disconnected){
            return;
        }
        send(m_client_socket, message.c_str(), message.length(), 0);
    }

    const void write(DogeType* object){
        write(object->serialize());
    }









    const bool isDisconnected(){
        return m_disconnected;
    }


    void stop(){
        close(m_client_socket);
        shutdown(m_server_socket, SHUT_RDWR);
        m_disconnected = true;
    }


    ~Socket(){
        stop();
    }
};

#endif DOGEBASE_SOCKET_HPP




