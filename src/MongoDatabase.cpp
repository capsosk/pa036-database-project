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
    _database = _client["mydb"];
}

std::string MongoDatabase::GetName()
{
    return "MongoDB";
}

bool MongoDatabase::AddMultipleObjects(const jsonObjects &vector)
{
    std::vector<bsoncxx::document::value> documents;

    //! convert to BSON
    for (const auto &jsonItem : vector) {
        auto bsonValue = bsoncxx::from_json(jsonItem);
        documents.push_back(bsonValue);
    }

    auto collection = _database["test"];
    auto result = collection.insert_many(documents);

    //! find every such as 10 < age <= 39
    mongocxx::cursor cursor = collection.find(document{} << "age" << open_document << "$gt" << 10 << "$lte" << 39
                                                         << close_document << finalize);

    //! print it out as json
    for (auto doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << "\n";
    }

    //! cleanup db
    auto deleteResult =
            collection.delete_many({});

    return true;
}

bool MongoDatabase::RunQuery(const std::string &query)
{
    //! changes query for one json object and insert it
    auto value = bsoncxx::from_json(query);
    std::vector<bsoncxx::document::value> documents;
    documents.push_back(value);

    auto collection = _database["test"];
    auto result = collection.insert_many(documents);

    //! prints out single found value
    auto foundValue = collection.find_one({});
    std::cout << bsoncxx::to_json(foundValue->view()) << "\n";

    //! cleanup the db
    auto deleteResult =
            collection.delete_many({});

    return false;
}
