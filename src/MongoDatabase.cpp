//
// Created by capsosk on 16/04/2021.
//

#include "../headers/MongoDatabase.h"

#include <iostream>
#include "../headers/ChronoWrapper.h"

MongoDatabase::MongoDatabase(const std::string &db_address)
{
    mongocxx::uri uri(db_address);
    _client = { uri };
    if (!_client) {
        std::cerr << "Postgresql failed to init" << '\n';
    }
    _database = _client[kDbName];
    collection = _database[kTableName];
}

std::string MongoDatabase::GetName()
{
    return "MongoDB";
}

void MongoDatabase::UpdateMany(const bsoncxx::document::value &toUpdate, const bsoncxx::document::value &setTo)
{
    std::fstream stream("update10000index", std::ios_base::app);
    auto chrono = ChronoWrapper();
    chrono.startTimer();
    collection.update_many({ toUpdate }, { setTo });
    chrono.endTimer();
    chrono.displayResultOnStream(stream);
    stream.flush();
}

void MongoDatabase::Aggregate(const bsoncxx::document::value &matchValue, const bsoncxx::document::value &groupValue)
{
    mongocxx::pipeline stages;
    stages.match({ matchValue }).group({ groupValue });
    auto cursor = collection.aggregate(stages);
    //    std::cout << "\nPrinting Aggregate: \n";
    //    PrintCursor(cursor);
}

void MongoDatabase::AddMultipleObjects(const FileParser::jsonObjects &vector)
{
    std::vector<bsoncxx::document::value> documents;
    //! convert to BSON
    for (const auto &jsonItem : vector) {
        auto bsonValue = bsoncxx::from_json(jsonItem);
        documents.push_back(bsonValue);
    }
    auto result = collection.insert_many(documents);
}

void MongoDatabase::ClearDatabase()
{
    collection.delete_many({});
}

void MongoDatabase::DropDatabase()
{
    collection.drop();
}

void MongoDatabase::CreateIndexes(const bsoncxx::document::value &indexes)
{
    collection.create_index({ indexes });
}

void MongoDatabase::FindMany(const bsoncxx::document::value &toFind)
{
    mongocxx::cursor cursor = collection.find({ toFind });
    //    std::cout << "\nPrinting FindMany: \n";
    //    PrintCursor(cursor);
    //    std::cout << '\n';
}

void MongoDatabase::PrintCursor(mongocxx::cursor &cursor)
{
    uint size = 0;
    for (const auto &doc : cursor) {
        size++;
        std::cout << bsoncxx::to_json(doc) << "\n";
    }
    std::cout << "\nTotal Printed: " << size << '\n';
}
