//
// Created by capsosk on 16/04/2021.
//

#include "../headers/PostgresqlDatabase.h"

#include <iostream>

PostgresqlDatabase::PostgresqlDatabase(const std::string &name, const std::string &db_address)
    : DatabaseBase(name, db_address), c(db_address)
{
    if (!c.is_open()) {
        std::cerr << "PostgreSQL Initialization failed!" << '\n';
    }
}

bool PostgresqlDatabase::RunQuery(const std::string &query)
{
    pqxx::work txn{ c };
    /** do stuff with txn http://pqxx.org/development/libpqxx/#quick-example **/
    return false;
}