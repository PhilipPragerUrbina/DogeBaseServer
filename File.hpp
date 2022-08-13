//
// Created by Philip on 8/8/2022.
//

#ifndef DOGEBASE_FILE_HPP
#define DOGEBASE_FILE_HPP


#include <fstream>
#include "DogeType.hpp"

//class for file IO

class File {
private:
    std::string  m_filename;
public:
    File(std::string filename){
        m_filename = filename;
    }

    //append a new item to the file
    void writeItem(std::string bytes){
        std::ofstream output (m_filename,std::ofstream::app| std::fstream::binary );
        //write size then actual data
        output  << DogeInt(bytes.size()).serialize() <<bytes;
        output.close();
    }
    void writeItem(DogeType* object){
        writeItem(object->serialize());
    }

    void overWriteItem(std::string bytes, int offset = 0){
        if(offset == 0){
            std::ofstream output (m_filename, std::fstream::binary );
            output  << DogeInt(bytes.size()).serialize() <<bytes;
            output.close();
        }else{
            std::fstream output( m_filename, std::fstream::in | std::fstream::out | std::fstream::binary );
            output.seekp( offset );
            output  << DogeInt(bytes.size()).serialize() <<bytes;
            output.close();
        }
    }
    void overWriteItem(DogeType* object, int offset=0){
        overWriteItem(object->serialize(), offset);
    }
    //read nth item
   const  std::string readItem(int number){
        std::ifstream input (m_filename,std::fstream::binary );
        std::string out = "";

        for (int i = 0; i < number+1; ++i) {
            //read size of item
            char size_data [4];
            if(!input.read(size_data, 4)){
                break;
            };
            int size = DogeInt(size_data);

            //read object data based on it's size
            char* object_data = new char [size];
            if(!input.read(object_data, size)){
                break;
            };
            //get object bytes if it matches the id
            if(i == number){
                out = std::string (object_data, size);
            }
            delete[] object_data;
        }
        input.close();
        return out;
    }


    void clear(){
        std::ofstream clear (m_filename);
        clear.close();
    }



};

#endif DOGEBASE_FILE_HPP