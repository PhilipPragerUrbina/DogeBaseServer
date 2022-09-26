//
// Created by Philip on 8/23/2022.
//

#ifndef DOGEBASE_TRANSACTIONMEMORY_HPP
#define DOGEBASE_TRANSACTIONMEMORY_HPP


#include <fstream>
#include "Socket.hpp"
#include "File.hpp"
#include "DataBase.hpp"
#include "Operation.hpp"


//represent temporary transaction memory for use during transaction
//for example, a transaction where there is a write, then a read, the write need to appear in the read
class TransactionMemory{
private:
    //data is stored in a map
    std::map<int, DogeType*> m_transaction_memory;

    int m_latest_id_offset = 0;//how many ids created so far
    //the lifetime of the data is tied to the transaction, so it does not need to be deleted here
public:
    int getIDOffset(){
        return m_latest_id_offset;
    }
    void incrementID(){
        m_latest_id_offset++;
    }

    DogeType* getMemory(int id){
        if (m_transaction_memory.find(id) == m_transaction_memory.end()) {
            return nullptr;
        }
        return m_transaction_memory.at(id);

    }
    DogeType* setMemory(int id, DogeType* data){
       m_transaction_memory[id] = data;
    }
};

#endif DOGEBASE_TRANSACTIONMEMORY_HPP