#include <iostream>
#include "getTime.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    struct tm* localTime = std::localtime(&time);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream fulltime;
    fulltime << std::put_time(localTime, "%H:%M:%S");
    fulltime << "." << std::setfill('0') << std::setw(3) << ms.count();
    return fulltime.str();
}