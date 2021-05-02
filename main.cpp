//
// Created by capsosk on 16/04/2021.
//

#include <iostream>

#include "headers/ChronoWrapper.h"
#include "headers/FileParser.h"
#include "headers/MongoDatabase.h"
#include "headers/PostgresqlDatabase.h"


int main(int /*argc*/, const char** argv)
{
    auto timer = ChronoWrapper();
    timer.startTimer();
    constexpr auto mongo_address = "mongodb://localhost:27017";
    constexpr auto pg_address = "postgresql://accounting@localhost/company";

    auto mongo = MongoDatabase(mongo_address);
    std::cout << mongo.GetName() << '\n';

    /*auto postgresql = PostgresqlDatabase(pg_address);
    std::cout << postgresql.GetName() << '\n';*/

    auto parser = FileParser(argv[1]);

    //! examples https://github.com/mongodb/mongo-cxx-driver/blob/master/examples/mongocxx/query.cpp

    const auto objects = parser.returnJsonObjects();

    mongo.AddMultipleObjects(objects);

    timer.endTimer();
    timer.displayResultOnCout();
    return 0;
}
