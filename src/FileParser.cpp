//
// Created by capsosk on 02/05/2021.
//

#include "../headers/FileParser.h"

FileParser::FileParser(const char *file)
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

jsonObjects FileParser::returnJsonObjects()
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(_fileStream, root);

    Json::FastWriter fastWriter;
    std::vector<std::string> items;

    for (const auto &item : root) {
        items.push_back(fastWriter.write(item));
    }

    return items;
}
