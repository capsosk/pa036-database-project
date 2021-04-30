//
// Created by capsosk on 16/04/2021.
//

#include "../headers/MongoDatabase.h"

#include <iostream>

MongoDatabase::MongoDatabase(const std::string &name, const std::string &db_address)
    : DatabaseBase(name, db_address)
{
    mongocxx::uri uri(db_address);
    c = { uri };
    if (!c) {
        std::cerr << "Postgresql failed to init" << '\n';
    }
}

bool MongoDatabase::RunQuery(const std::string &query)
{
    /** http://mongocxx.org/mongocxx-v3/tutorial/ **/
    return false;
}
