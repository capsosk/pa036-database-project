//
// Created by capsosk on 02/05/2021.
//

#pragma once

#include <fstream>
#include <jsoncpp/json/json.h>
#include <string>
#include <vector>

#include "DatabaseBase.h"

class FileParser
{
  public:
    FileParser(const char *file);
    [[nodiscard]] bool returnOneLine(std::string &line);
    void returnWholeFile(std::string &file);
    jsonObjects returnJsonObjects();

  private:
    std::ifstream _fileStream;
};
