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

    void AddOneObject(const std::string &query) override;
    void AddMultipleObjects(const std::vector<std::string> &vector) override;
    void ClearDatabase() override;

  private:
    pqxx::connection c;
};
