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
    DatabaseBase(const std::string &name, const std::string &)
        : name(name){};

    //! @brief runs query on database
    //! @param query: query to run
    //! @precondition: must be initialized
    virtual bool RunQuery(const std::string &query) = 0;

    virtual ~DatabaseBase() = default;

    std::string name;

  private:
    bool _initialized = false;
};
