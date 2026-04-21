#pragma once
#include <string>
#include <vector>
#include <regex>
#include "LogEntry.h"

class LogParser {
private:
    std::regex log_pattern;

public:
    LogParser();
    
    LogEntry parseLine(const std::string& line) const;
    
    std::vector<LogEntry> parseFile(const std::string& filepath) const;
};