//
// Created by capsosk on 02/05/2021.
//

#include <algorithm>
#include "../headers/FileParser.h"

FileParser::FileParser(const std::string &file)
{
    _fileStream = std::ifstream(file);
}
bool FileParser::returnOneLine(std::string &line)
{
    return static_cast<bool>(std::getline(_fileStream, line));
}

void FileParser::returnWholeFile(std::string &file)
{
    std::string line;
    while (static_cast<bool>(std::getline(_fileStream, line))) {
        file.append(line + '\n');
    };
}

FileParser::jsonObjects FileParser::returnJsonObjects()
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(_fileStream, root);
    _fileStream.seekg(0);

    Json::FastWriter fastWriter;
    std::vector<std::string> items;

    for (const auto &item : root) {
        items.push_back(fastWriter.write(item));
    }

    return items;
}

FileParser::JsonSchemeData FileParser::returnJsonSchemeObjects()
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(_fileStream, root);
    _fileStream.seekg(0);

    Json::FastWriter fastWriter;

    JsonSchemeData objects;

    for (const auto &item : root) {
        auto address = item["address"];
        auto job = item["job"];
        auto numbers = item["phoneNumbers"];

        Person person;

        person.name = item["name"].asString();
        person.surname = item["surname"].asString();
        person.birthdate = item["birthdate"].asString();

        person.address.city = address["city"].asString();
        person.address.street = address["street"].asString();
        person.address.streetNumber = address["streetNumber"].asInt();
        person.address.postalCode = address["postalCode"].asInt();

        person.job.companyName = job["companyName"].asString();
        person.job.salary = job["salary"].asInt();
        person.job.startDate = job["startDate"].asString();

        for (const auto &number : numbers) {
            person.phoneNumbers.push_back(number.asString());
        }
        objects.push_back(person);
    }

    return objects;
}
