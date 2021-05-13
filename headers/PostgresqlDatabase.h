//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include <pqxx/pqxx>
#include "FileParser.h"

class PostgresqlDatabase
{
  public:
    PostgresqlDatabase(const std::string &db_address);

    std::string GetName();

    void RunQuery(const std::string &query);
    void AddMultipleObjectsJson_b(const std::string &query);
    void AddMultipleObjectsScheme(const FileParser::jsonObjects &objects);
    void ClearDatabase();

  private:
    pqxx::connection c;
};
