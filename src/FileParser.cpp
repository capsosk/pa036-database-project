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