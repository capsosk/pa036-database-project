//
// Created by capsosk on 16/04/2021.
//

#pragma once

#include <chrono>
#include <ostream>

class ChronoWrapper
{
  public:
    ChronoWrapper() = default;

    void startTimer();

    void endTimer();

    void displayResultOnStream(std::ostream &out);

  private:
    std::chrono::system_clock::time_point dateStart;
    std::chrono::system_clock::time_point dateEnd;
    bool running = false;
};