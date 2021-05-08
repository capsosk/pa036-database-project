//
// Created by capsosk on 16/04/2021.
//

#include <iostream>

#include "headers/ChronoWrapper.h"
#include "headers/FileParser.h"
#include "headers/MongoDatabase.h"
#include "headers/PostgresqlDatabase.h"

constexpr auto kFileName = "../10000.json";

int main()
{
    auto timer = ChronoWrapper();

    constexpr auto mongo_address = "mongodb://localhost:27017";
    constexpr auto pg_address = "postgresql://testuser:qwerty@localhost/testdb";

    auto mongo = MongoDatabase(mongo_address);
    std::cout << mongo.GetName() << " initialized" << '\n';

    auto postgresql = PostgresqlDatabase(pg_address);
    std::cout << postgresql.GetName() << " initialized" << '\n';

    auto parser = FileParser(kFileName);

    //! examples https://github.com/mongodb/mongo-cxx-driver/blob/master/examples/mongocxx/query.cpp
    timer.startTimer();
    const auto objects = parser.returnJsonObjects();

    mongo.AddMultipleObjects(objects);

    MongoDatabase::ParamValue<int> indexes;
    indexes.push_back({ "birthdate", 1 });
    indexes.push_back({ "job.salary", 1 });
    mongo.CreateIndexes(indexes);

    //postgresql.AddMultipleObjects(objects);

    timer.endTimer();

    mongo.ClearDatabase();
    postgresql.ClearDatabase();

    std::fstream out("file", std::ios_base::app);
    out << "\nWhole program: \n";
    timer.displayResultOnStream(out);

    return 0;
}
