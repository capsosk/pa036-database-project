//
// Created by Jakub Novák @capsosk on 2019-05-30.
//

#pragma once

#include "../headers/ChronoWrapper.h"

#include <cassert>
#include <iostream>

void ChronoWrapper::displayResultOnStream(std::ostream &out)
{
    out << std::chrono::duration_cast<std::chrono::microseconds>(dateEnd - dateStart).count() << "\n";
}

void ChronoWrapper::endTimer()
{
    dateEnd = std::chrono::system_clock::now();
    std::cout << "Benchmark ended\n";
}

void ChronoWrapper::startTimer()
{
    assert(!running && "timer is already running");
    dateStart = std::chrono::system_clock::now();
    std::cout << "Benchmark started\n";
}