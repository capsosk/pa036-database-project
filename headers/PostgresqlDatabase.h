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
    void AddMultipleObjectsJsonb(const FileParser::jsonObjects &objects);
    void AddMultipleObjectsJson(const FileParser::jsonObjects &objects);
    void AddMultipleObjectsScheme(const FileParser::jsonObjects &objects);
    void ClearDatabase();

  private:
    pqxx::connection c;
};
