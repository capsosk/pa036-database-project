//
// Created by capsosk on 16/04/2021.
//

#include <iostream>

#include "headers/ChronoWrapper.h"
#include "headers/MongoDatabase.h"
#include "headers/PostgresqlDatabase.h"

int main()
{
    auto timer = ChronoWrapper();
    timer.startTimer();
    constexpr auto mongo_address = "postgresql://accounting@localhost/company";
    constexpr auto pg_address = "mongodb://localhost:27017";

    MongoDatabase mongo = MongoDatabase("MongoDB", mongo_address);
    std::cout << mongo.name << '\n';

    PostgresqlDatabase postgresql = PostgresqlDatabase("PostgreSQLDB", pg_address);
    std::cout << postgresql.name << '\n';

    timer.endTimer();
    timer.displayResultOnCout();
    return 0;
}
