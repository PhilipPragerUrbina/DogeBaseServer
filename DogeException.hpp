//
// Created by Philip on 8/21/2022.
//

#ifndef DOGEBASE_DOGEEXCEPTION_HPP
#define DOGEBASE_DOGEEXCEPTION_HPP

#include <string>

//class for doge errors and exceptions
class DogeException {

public:
    DogeException(std::string message){
        m_message = message;
    }
    DogeException(){
        m_message = "Unknown error";
    }
     std::string getMessage(){
         return m_message;
    }
private:
    std::string m_message;



};

#endif DOGEBASE_DOGEEXCEPTION_HPP