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

void PostgresqlDatabase::AddOneObject(const std::string &query)
{
    pqxx::work txn{ c };
    txn.exec(query);
    txn.commit();
}

std::string PostgresqlDatabase::GetName()
{
    return "PostgreSQL";
}

void PostgresqlDatabase::ClearDatabase()
{
    static const std::string clearQuery = "DROP OWNED BY testuser";
    pqxx::work txn{ c };
    txn.exec(clearQuery);
    txn.commit();
}

void PostgresqlDatabase::AddMultipleObjects(const std::vector<std::string> &objects)
{
    std::string begin = "INSERT INTO table VALUES";
    std::string core;
    for (auto &item : objects) {
        core.append(std::to_string('(') + item + "),");
    }
    core.append(";");
    pqxx::work txn{ c };
    txn.exec(begin + core);
    txn.commit();
}
