//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include "DatabaseBase.h"

#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

class MongoDatabase : public DatabaseBase
{
  public:
    MongoDatabase(const std::string &db_address);
    ~MongoDatabase() override = default;

    std::string GetName() override;

    bool RunQuery(const std::string &query) override;
    bool AddMultipleObjects(const std::vector<std::string> &vector);

  private:
    [[maybe_unused]] const mongocxx::instance instance{};
    mongocxx::client _client;
    mongocxx::database _database;
};
