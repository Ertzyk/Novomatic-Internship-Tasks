#include "LogParser.h"
#include <fstream>
#include <iostream>

// Expected format: [TIMESTAMP] [LEVEL] [SOURCE] Message
LogParser::LogParser() : log_pattern(R"(^\[(.*?)\]\s+\[(.*?)\]\s+\[(.*?)\]\s+(.*)$)") {}

LogEntry LogParser::parseLine(const std::string& line) const {
    std::smatch matches;

    // Extract the 4 capture groups
    if(std::regex_match(line, matches, log_pattern)){
        return {
            matches[1].str(),
            matches[2].str(),
            matches[3].str(),
            matches[4].str()
        };
    }
    
    return {}; 
}

std::vector<LogEntry> LogParser::parseFile(const std::string& filepath) const {
    std::vector<LogEntry> logs;
    std::ifstream file(filepath);
    std::string line;

    if(!file.is_open()){
        std::cerr << "Error opening file: " << filepath << "\n";
        return logs;
    }

    while(std::getline(file, line)){
        if (line.empty()) continue;
        
        LogEntry entry = parseLine(line);
        // Only append successfully parsed entries
        if(!entry.timestamp.empty()){
            logs.push_back(entry);
        }
    }

    return logs;
}