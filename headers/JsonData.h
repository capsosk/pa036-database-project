//
// Created by capsosk on 11/05/2021.
//

#pragma once

struct Address
{
    std::string city;
    std::string street;
    int streetNumber;
    int postalCode;
};

struct Job
{
    std::string companyName;
    int salary;
    std::string startDate;
};

struct Person
{
    std::string name;
    std::string surname;
    std::string birthdate;
    Address address;
    Job job;
    std::vector<std::string> phoneNumbers;
};
