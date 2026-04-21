#pragma once
#include <vector>
#include <string>
#include "LogEntry.h"

struct Query {
    std::string level = "";
    std::string source = "";
    std::string message_contains = "";
    std::string start_time = "";
    std::string end_time = "";
};

class QueryEngine {
public:
    static std::vector<LogEntry> filterLogs(const std::vector<LogEntry>& logs, const Query& q);
};