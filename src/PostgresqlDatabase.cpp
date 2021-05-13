//
// Created by capsosk on 16/04/2021.
//

#include "../headers/PostgresqlDatabase.h"
#include "../PostgresQueries.h"

#include <iostream>

PostgresqlDatabase::PostgresqlDatabase(const std::string &db_address)
    : c(db_address)
{
    if (!c.is_open()) {
        std::cerr << "PostgreSQL Initialization failed!" << '\n';
    }
}

void PostgresqlDatabase::RunQuery(const std::string &query)
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

void PostgresqlDatabase::AddMultipleObjectsJson_b(const std::string &query)
{
    pqxx::work txn{ c };
    txn.exec(query);
    txn.commit();
}

void PostgresqlDatabase::AddMultipleObjectsScheme(const FileParser::jsonObjects &objects)
{
    for (const auto &person : objects) {
        /** this might be problematic, we are running query for each object,
         ** as we couldnt figure out how to add more at once **/
        RunQuery(person);
    }
}
