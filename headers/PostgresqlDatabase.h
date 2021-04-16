//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include "DatabaseBase.h"

class PostgresqlDatabase : public DatabaseBase
{
  public:
    PostgresqlDatabase(const std::string &name);
    ~PostgresqlDatabase() override = default;

    bool RunQuery(const std::string &query) override;

  private:
    bool InitDatabase() override;
};
