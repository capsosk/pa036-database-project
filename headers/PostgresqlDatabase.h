//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include "DatabaseBase.h"

#include <pqxx/pqxx>

class PostgresqlDatabase : public DatabaseBase
{
  public:
    PostgresqlDatabase(const std::string &db_address);
    ~PostgresqlDatabase() override = default;

    std::string GetName() override;

    bool RunQuery(const std::string &query) override;

  private:
    pqxx::connection c;
};
