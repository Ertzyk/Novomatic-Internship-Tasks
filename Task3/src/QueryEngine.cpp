#include "QueryEngine.h"

std::vector<LogEntry> QueryEngine::filterLogs(const std::vector<LogEntry>& logs, const Query& q){
    std::vector<LogEntry> results;

    for(const auto& log: logs){
        //evaluate and reject non-matching logs

        if(!q.level.empty() && log.level != q.level){
            continue;
        }

        if(!q.source.empty() && log.source != q.source){
            continue;
        }

        // Substring search for partial message matching
        if(!q.message_contains.empty() && log.message.find(q.message_contains) == std::string::npos){
            continue;
        }

        if(!q.start_time.empty() && log.timestamp < q.start_time){
            continue;
        }

        if(!q.end_time.empty() && log.timestamp > q.end_time){
            continue;
        }
        
        results.push_back(log);
    }

    return results;
}