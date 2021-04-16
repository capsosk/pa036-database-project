//
// Created by capsosk on 16/04/2021.
//

#include "../headers/MongoDatabase.h"

MongoDatabase::MongoDatabase(const std::string &name)
    : DatabaseBase(name)
{
}

bool MongoDatabase::InitDatabase()
{
    return false;
}

bool MongoDatabase::RunQuery(const std::string &query)
{
    return false;
}
