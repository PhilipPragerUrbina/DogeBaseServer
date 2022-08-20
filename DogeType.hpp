//
// Created by Philip on 8/3/2022.
//

#ifndef DOGEBASE_DOGETYPE_HPP
#define DOGEBASE_DOGETYPE_HPP

#include <string>
//this file has all the types that can be represented in the doge m_data
//it is mirrored in the java client such that these types can be seamlessly interchanged

enum DogeTypeID {
    DOGE_FILE,   //A separate file. Basically data of a certain size under a filename.
    DOGE_DATA,   //raw bytes
    DOGE_STRING,  //any text
    DOGE_INTEGER,  //a number
    DOGE_DOUBLE, //a floating point number
    DOGE_DATE,  //A date, like the time of update
    DOGE_URL,  //a URL
    DOGE_LINK, //reference to other location in m_data
    DOGE_OBJECT // a java object
};



//m_data_base class
 class DogeType {
 protected:
     DogeTypeID m_id = DOGE_DATA;
public:

    virtual const std::string serialize() {};
     virtual  DogeTypeID getID(){ return m_id;};
};

//doge integer
class DogeInt : public DogeType{
private:

    int32_t m_value;
public:


    DogeInt(int value){
        m_id = DOGE_INTEGER;
        m_value = value;
    }

    DogeInt(std::string bytes){
        m_id = DOGE_INTEGER;
        if(bytes.size() == 0){
            m_value = 0;
            return;
        }
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
    operator int() const {return m_value;}


};

//doge string or text
class DogeString : public DogeType{
private:

    std::string m_value;
public:
    //bytes and raw value are the same
    DogeString(std::string chars){
         m_id = DOGE_STRING;
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
    std::string m_value;
public:
    //you cannot access the value of this since it is from java
    DogeObject(std::string chars){
        m_id = DOGE_OBJECT;
        m_value = chars;
    }
    const std::string serialize() override{
        return m_value;
    }
};

class DogeTypeData{
private:
    DogeType* m_data = nullptr;
    DogeTypeID m_type;
public:
    DogeTypeData(std::string bytes , DogeTypeID type){
        m_type = type;
        switch (m_type) {
            case DOGE_OBJECT:
                m_data = new DogeObject(bytes);
            case DOGE_DATA:
                m_data = new DogeObject(bytes);
            case DOGE_INTEGER:
                m_data = new DogeInt(bytes);
            case DOGE_STRING:
                m_data = new DogeString(bytes);
        }
    }

    DogeTypeData(DogeType* data){
        m_data = data;
        m_type = data->getID();
    }

    DogeType* getData(){
        return m_data;
    }

    DogeTypeID getType(){
        return m_type;
    }




    ~DogeTypeData(){
        if(m_data != nullptr){
            delete m_data;
        }
    }


};

#endif DOGEBASE_DOGETYPE_HPP