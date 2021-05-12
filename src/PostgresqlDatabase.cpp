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

void PostgresqlDatabase::AddMultipleObjectsJsonb(const std::vector<std::string> &objects)
{
    std::string begin = "INSERT INTO person_jsonb VALUES";
    std::string core;
    for (auto &item : objects) {
        core.append("(\'" + item + "\'::jsonb),");
    }
    core.pop_back();
    core.append(";");
    pqxx::work txn{ c };
    txn.exec(begin + core);
    txn.commit();
}

void PostgresqlDatabase::AddMultipleObjectsJson(const FileParser::jsonObjects &objects)
{
    std::string begin = "INSERT INTO person_json VALUES";
    std::string core;
    for (auto &item : objects) {
        core.append("(\'" + item + "\'),");
    }
    core.pop_back();
    core.append(";");
    pqxx::work txn{ c };
    txn.exec(begin + core);
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
