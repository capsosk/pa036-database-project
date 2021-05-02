//
// Created by capsosk on 16/04/2021.
//

#include "../headers/PostgresqlDatabase.h"

#include <iostream>

PostgresqlDatabase::PostgresqlDatabase(const std::string &db_address)
    : DatabaseBase(db_address), c(db_address)
{
    if (!c.is_open()) {
        std::cerr << "PostgreSQL Initialization failed!" << '\n';
    }
}

bool PostgresqlDatabase::RunQuery(const std::string &query)
{
    pqxx::work txn{ c };
    txn.exec(query);
    txn.commit();
    return true;
}

std::string PostgresqlDatabase::GetName()
{
    return "PostgreSQL";
}
