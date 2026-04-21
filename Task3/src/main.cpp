#include <iostream>
#include "LogParser.h"
#include "QueryEngine.h"

int main() {
    LogParser parser;
    auto logs = parser.parseFile("logs.txt");

    if (logs.empty()) {
        std::cerr << "No logs loaded\n";
        return 1;
    }

    // Example query configuration
    Query q;
    q.level = "ERROR";

    std::cout << "Executing Query: LEVEL=\"ERROR\" ---\n";
    auto results = QueryEngine::filterLogs(logs, q);

    if(results.empty()){
        std::cout << "No matches found\n";
    } else {
        for(const auto& log : results){
            std::cout << log.toString() << "\n";
        }
    }

    return 0;
}