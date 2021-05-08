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

    auto indexes = make_document(kvp("birthdate", 1), kvp("job.salary", 1));
    mongo.CreateIndexes(indexes);

    auto filter = document{} << "birthdate" << open_document
                             << "$gt"
                             << "1970-01-01T00:00:00.000Z"
                             << "$lte"
                             << "1980-01-01T00:00:00.000Z"
                             << close_document << finalize;
    mongo.FindMany(filter);

    auto valueMatch = make_document(kvp("job.salary", make_document(kvp("$lt", 1000))));
    auto valueGroup = make_document(kvp("_id", make_document(kvp("birthdate", make_document(kvp("year", "$birthdate"))))),
            kvp("avg_salary", make_document(kvp("$avg", "$job.salary"))));

    mongo.Aggregate(valueMatch, valueGroup);

    auto toUpdate = make_document(kvp("birthdate", make_document(kvp("$lt", "1970-01-01T00:00:00.000Z"))));
    auto setTo = make_document(kvp("$set", make_document(kvp("job.salary", 3000))));

    mongo.UpdateMany(toUpdate, setTo);

    //postgresql.AddMultipleObjects(objects);

    timer.endTimer();

    mongo.ClearDatabase();
    postgresql.ClearDatabase();

    std::fstream out("file", std::ios_base::app);
    out << "\nWhole program: \n";
    timer.displayResultOnStream(out);

    return 0;
}
