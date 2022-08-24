//
// Created by Philip on 8/12/2022.
//

#ifndef DOGEBASE_DATABASE_HPP
#define DOGEBASE_DATABASE_HPP


#include "File.hpp"
//represents the associated data with a database
class DataBase {
public:
    //add transaction memory here as a vecxtor
    File m_data;
    File m_metadata;
    DataBase(std::string name): m_data(name + ".dogetable"), m_metadata(name+".dogemeta"){
        m_name = name;
    }
private:
    std::string m_name;
};

#endif DOGEBASE_DATABASE_HPP