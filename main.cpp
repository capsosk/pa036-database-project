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
    constexpr auto mongo_address = "postgresql://accounting@localhost/company";
    constexpr auto pg_address = "mongodb://localhost:27017";

    MongoDatabase mongo = MongoDatabase(mongo_address);
    std::cout << mongo.GetName() << '\n';

    PostgresqlDatabase postgresql = PostgresqlDatabase(pg_address);
    std::cout << postgresql.GetName() << '\n';

    FileParser parser {argv[1]};
    for (std::string line; parser.returnOneLine(line); ) {
        std::cout << line << '\n';
    }

    timer.endTimer();
    timer.displayResultOnCout();
    return 0;
}
