//
// Created by Philip on 8/3/2022.
//

#ifndef DOGEBASE_DOGETYPE_HPP
#define DOGEBASE_DOGETYPE_HPP

#include <string>
//this file has all the types that can be represented in the doge database
//it is mirrored in the java client such that these types can be seamlessly interchanged

enum DogeTypeID {
    DOGE_FILE,   //A separate file. Basically data of a certain size under a filename.
    DOGE_DATA,   //raw bytes
    DOGE_STRING,  //any text
    DOGE_INTEGER,  //a number
    DOGE_DOUBLE, //a floating point number
    DOGE_DATE,  //A date, like the time of update
    DOGE_URL,  //a URL
    DOGE_LINK, //reference to other location in database
    DOGE_OBJECT // a java object
};

//base class
 class DogeType {
public:
    virtual const std::string serialize() {};
};

//doge integer
class DogeInt : public DogeType{
private:
    DogeTypeID m_id = DOGE_INTEGER;
    int32_t m_value;
public:

    DogeInt(int value){
        m_value = value;
    }

    DogeInt(std::string bytes){
        //convert bytes to signed int
        m_value = *(int32_t *)bytes.data();
    }

    const std::string serialize() override{
        char* char_buffer = static_cast<char*>(static_cast<void*>(&m_value));

        return std::string(char_buffer, 4);
    }

   const int getValue(){
        return m_value;
    }
};

//doge string or text
class DogeString : public DogeType{
private:
    DogeTypeID m_id = DOGE_STRING;
    std::string m_value;
public:
    //bytes and raw value are the same
    DogeString(std::string chars){
        m_value = chars;
    }
    const std::string serialize() override{
        return m_value;
    }
    const  std::string getValue(){
        return m_value;
    }
};

//doge java object
class DogeObject : public DogeType{
private:
    DogeTypeID m_id = DOGE_OBJECT;
    std::string m_value;
public:
    //you cannot access the value of this since it is from java
    DogeObject(std::string chars){
        m_value = chars;
    }
    const std::string serialize() override{
        return m_value;
    }
};


#endif DOGEBASE_DOGETYPE_HPP