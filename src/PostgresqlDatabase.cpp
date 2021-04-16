//
// Created by capsosk on 16/04/2021.
//

#include "../headers/PostgresqlDatabase.h"

PostgresqlDatabase::PostgresqlDatabase(const std::string &name)
    : DatabaseBase(name)
{
}

bool PostgresqlDatabase::InitDatabase()
{
    return false;
}

bool PostgresqlDatabase::RunQuery(const std::string &query)
{
    return false;
}