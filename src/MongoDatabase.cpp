//
// Created by capsosk on 16/04/2021.
//

#include "../headers/MongoDatabase.h"

#include <iostream>

MongoDatabase::MongoDatabase(const std::string &db_address)
    : DatabaseBase(db_address)
{
    mongocxx::uri uri(db_address);
    _client = { uri };
    if (!_client) {
        std::cerr << "Postgresql failed to init" << '\n';
    }
    _database = _client[kDbName];
}

std::string MongoDatabase::GetName()
{
    return "MongoDB";
}
bool MongoDatabase::UpdateOne(const std::string &toUpdate)
{
}

bool MongoDatabase::UpdateMany(const std::vector<std::string> &toUpdate)
{
}

void MongoDatabase::AddMultipleObjects(const jsonObjects &vector)
{
    std::vector<bsoncxx::document::value> documents;

    //! convert to BSON
    for (const auto &jsonItem : vector) {
        auto bsonValue = bsoncxx::from_json(jsonItem);
        documents.push_back(bsonValue);
    }

    auto collection = _database[kTableName];
    auto result = collection.insert_many(documents);

    //! find every such as 10 < age <= 39
    mongocxx::cursor cursor = collection.find(document{} << "age" << open_document << "$gt" << 0 << "$lte" << 20
                                                         << close_document << finalize);

    //! print it out as json
    for (auto doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << "\n";
    }

    //! cleanup db
    //    auto deleteResult =
    //            collection.delete_many({});
}

void MongoDatabase::AddOneObject(const std::string &query)
{
    //! changes query for one json object and insert it
    auto value = bsoncxx::from_json(query);
    std::vector<bsoncxx::document::value> documents;
    documents.push_back(value);

    auto collection = _database[kTableName];
    auto result = collection.insert_many(documents);

    //! prints out single found value
    auto foundValue = collection.find_one({});
    std::cout << bsoncxx::to_json(foundValue->view()) << "\n";

    //! cleanup the db
    //    auto deleteResult =
    //            collection.delete_many({});
}

void MongoDatabase::ClearDatabase()
{
    auto collection = _database[kTableName];
    [[maybe_unused]] auto deleteResult = collection.delete_many({});
}

void MongoDatabase::CreateIndexes(const ParamValue<int> &indexes)
{
    auto collection = _database[kTableName];
    for (const auto &index : indexes) {
        collection.create_index(make_document(kvp(index.first, index.second)));
    }
}

void MongoDatabase::FindMany(const std::string &toFind)
{
    auto collection = _database[kTableName];

    //! find every such as 10 < age <= 39
    mongocxx::cursor cursor = collection.find(document{} << "age" << open_document << "$gt" << 0 << "$lte" << 20
                                                         << close_document << finalize);
}
