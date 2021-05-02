//
// Created by capsosk on 02/05/2021.
//

#pragma once

#include <fstream>
#include <string>

class FileParser
{
  public:
    FileParser(const char *file);
    [[nodiscard]] bool returnOneLine(std::string &line);

  private:
    std::ifstream _fileStream;
};
