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
    MongoDatabase mongo = MongoDatabase(std::string("MongoDB"));
    std::cout << mongo.name << '\n';

    PostgresqlDatabase postgresql = PostgresqlDatabase(std::string("PostgreSQLDB"));
    std::cout << postgresql.name << '\n';
    timer.endTimer();
    timer.displayResultOnCout();
    return 0;
}
