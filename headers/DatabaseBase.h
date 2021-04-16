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
    DatabaseBase(const std::string &name)
        : name(name){};

    //! @brief runs query on database
    //! @param query: query to run
    //! @precondition: must be initialized
    virtual bool RunQuery(const std::string &query) = 0;

    virtual ~DatabaseBase() = default;

    std::string name;

  private:
    //! @brief Initializes the databse
    virtual bool InitDatabase() = 0;

    bool _initialized = false;
};
