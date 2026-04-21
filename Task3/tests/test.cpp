#include <gtest/gtest.h>
#include "../src/LogParser.h"
#include "../src/QueryEngine.h"

std::vector<LogEntry> getLogs(){
    return {
        {"2023-10-25T10:00:00", "INFO", "AuthService", "User logged in successfully"},
        {"2023-10-25T10:05:12", "ERROR", "Database", "Connection timeout"},
        {"2023-10-25T10:15:30", "WARN", "AuthService", "Multiple failed login attempts"},
        {"2023-10-25T10:20:00", "ERROR", "Payment", "Transaction rejected: insufficient funds"},
        {"2023-10-25T10:25:00", "INFO", "Payment", "Transaction 12345 processed"}
    };
}

// Przykład 1: Filtrowanie proste po jednym kluczu (Source)
TEST(QueryEngineTests, Przyklad1) {
    auto logs = getLogs();
    Query q;
    q.source = "AuthService";

    auto results = QueryEngine::filterLogs(logs, q);

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].timestamp, "2023-10-25T10:00:00");
    EXPECT_EQ(results[1].timestamp, "2023-10-25T10:15:30");
}

// Przykład 2: Kombinacja kluczy i przedziału czasu
TEST(QueryEngineTests, Przyklad2) {
    auto logs = getLogs();
    Query q;
    q.level = "ERROR";
    q.start_time = "2023-10-25T10:00:00";
    q.end_time = "2023-10-25T10:10:00";

    auto results = QueryEngine::filterLogs(logs, q);

    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].source, "Database");
    EXPECT_EQ(results[0].message, "Connection timeout");
}

// Przykład 3: Częściowe dopasowanie wiadomości
TEST(QueryEngineTests, Przyklad3) {
    auto logs = getLogs();
    Query q;
    q.message_contains = "Transaction";

    auto results = QueryEngine::filterLogs(logs, q);

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].source, "Payment");
    EXPECT_EQ(results[1].source, "Payment");
}