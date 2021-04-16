//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include "DatabaseBase.h"

class MongoDatabase : public DatabaseBase
{
  public:
    MongoDatabase(const std::string &name);
    ~MongoDatabase() override = default;

    bool RunQuery(const std::string &query) override;

  private:
    bool InitDatabase() override;
};
