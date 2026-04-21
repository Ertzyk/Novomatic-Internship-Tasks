#pragma once
#include <string>

struct LogEntry {
    std::string timestamp;
    std::string level;
    std::string source;
    std::string message;

    std::string toString() const{
        return "[" + timestamp + "] [" + level + "] [" + source + "] " + message;
    }
};