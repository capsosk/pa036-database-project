//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include <string>

//! @brief Base class for both databases
class DatabaseBase
{
  public:
    //! @brief constructor
    DatabaseBase(const std::string &){};

    //! @brief runs query on database
    //! @param query: query to run
    //! @precondition: must be initialized
    virtual bool RunQuery(const std::string &query) = 0;

    //! @brief return Database name
    virtual std::string GetName() = 0;

    virtual ~DatabaseBase() = default;
};
