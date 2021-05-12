//
// Created by capsosk on 16/04/2021.
//

#pragma once

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
#include "FileParser.h"

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

class MongoDatabase
{
  public:
    MongoDatabase(const std::string &db_address);

    std::string GetName();

    void AddMultipleObjects(const FileParser::jsonObjects &vector);
    void UpdateMany(const bsoncxx::document::value &toUpdate, const bsoncxx::document::value &setTo);
    void FindMany(const bsoncxx::document::value &toFind);
    void Aggregate(const bsoncxx::document::value &matchValue, const bsoncxx::document::value &groupValue);

    //! @brief also clears indexes
    void DropDatabase();

    void CreateIndexes(const bsoncxx::document::value &indexes);
    void ClearDatabase();

  private:
    static void PrintCursor(mongocxx::cursor &cursor);

    [[maybe_unused]] const mongocxx::instance instance{};
    mongocxx::client _client;
    mongocxx::database _database;
    mongocxx::v_noabi::collection collection;
};
