//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include <string>
#include <vector>

using jsonObjects = std::vector<std::string>;

//! @brief Base class for both databases
class DatabaseBase
{
  public:
    //! @brief constructor
    DatabaseBase(const std::string &){};

    //! @brief runs query on database
    //! @param query: query to run
    //! @precondition: must be initialized
    virtual void AddOneObject(const std::string &query) = 0;

    virtual void AddMultipleObjects(const std::vector<std::string> &vector) = 0;

    //! Clear all entries
    virtual void ClearDatabase() = 0;

    //! @brief return Database name
    virtual std::string GetName() = 0;

    virtual ~DatabaseBase() = default;
};
