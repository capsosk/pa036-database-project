//
// Created by capsosk on 16/04/2021.
//

#include "../headers/MongoDatabase.h"

#include <iostream>

MongoDatabase::MongoDatabase(const std::string &db_address)
    : DatabaseBase(db_address)
{
    mongocxx::uri uri(db_address);
    c = { uri };
    if (!c) {
        std::cerr << "Postgresql failed to init" << '\n';
    }
}

std::string MongoDatabase::GetName()
{
    return "MongoDB";
}

bool MongoDatabase::RunQuery(const std::string &query)
{
    /** http://mongocxx.org/mongocxx-v3/tutorial/ **/
    return false;
}
