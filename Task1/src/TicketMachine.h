#pragma once
#include "Server.h"
#include <map>
#include <vector>
#include <optional>

class TicketMachine {
private:
    Server& server;
    std::map<int, int> coin_inventory; 
    int timeout_ms;

public:
    TicketMachine(Server& s, std::map<int, int> initial_coins, int timeout_ms = 60000);

    int select_ticket(int ticket_price);

    std::optional<std::map<int, int>> calculate_change(int amoun_paid, int ticket_price);

    bool pay(int reservation_id, int ticket_price, int amount_paid, const UserData& user);

    void cancel_transaction(int reservationId);
};