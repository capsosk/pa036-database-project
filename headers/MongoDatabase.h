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

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

constexpr auto kTableName = "test";
constexpr auto kDbName = "mydb";

class MongoDatabase : public DatabaseBase
{
  public:
    template <typename T>
    using ParamValue = std::vector<std::pair<std::string, T>>;

    MongoDatabase(const std::string &db_address);
    ~MongoDatabase() override = default;

    std::string GetName() override;

    void AddOneObject(const std::string &query) override;
    void AddMultipleObjects(const std::vector<std::string> &vector) override;
    bool UpdateMany(const std::vector<std::string> &toUpdate);
    bool UpdateOne(const std::string &toUpdate);
    void FindMany(const std::string &toFind);

    void CreateIndexes(const ParamValue<int> &indexes);
    void ClearDatabase() override;

  private:
    [[maybe_unused]] const mongocxx::instance instance{};
    mongocxx::client _client;
    mongocxx::database _database;
};
