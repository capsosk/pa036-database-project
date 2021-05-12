//
// Created by capsosk on 02/05/2021.
//

#pragma once

#include <fstream>
#include <jsoncpp/json/json.h>
#include <string>
#include <vector>

#include "JsonData.h"

class FileParser
{
  public:
    using JsonSchemeData = std::vector<Person>;
    using jsonObjects = std::vector<std::string>;

    FileParser(const std::string &);
    [[nodiscard]] bool returnOneLine(std::string &line);
    void returnWholeFile(std::string &file);

    [[nodiscard]] jsonObjects returnJsonObjects();
    [[nodiscard]] JsonSchemeData returnJsonSchemeObjects();

  private:
    std::ifstream _fileStream;
};
