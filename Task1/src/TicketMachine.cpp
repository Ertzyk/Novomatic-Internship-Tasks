#include "TicketMachine.h"
#include <optional>
#include <thread>
#include <chrono>

TicketMachine::TicketMachine(Server& s, std::map<int, int> initial_coins, int timeout) : server(s), coin_inventory(initial_coins), timeout_ms(timeout) {}

void timeout_task(Server& server, int reservation_id, int wait_time_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time_ms));
    server.cancelReservation(reservation_id); 
}

int TicketMachine::select_ticket(int ticket_price){
    int reservation_id = server.reserveTicket();

    if(reservation_id != -1){
        // forget a background thread to handle the timeout cancellation
        std::thread timer_thread(timeout_task, std::ref(server), reservation_id, timeout_ms);
        timer_thread.detach();
    }

    return reservation_id;
}

std::optional<std::map<int, int>> TicketMachine::calculate_change(int amount_paid, int ticket_price){
    std::map<int, int> change;
    int diff = amount_paid - ticket_price;
    if(diff < 0) return std::nullopt;
    if(diff == 0) return change;

    std::vector<int> coins;

    // flatten the coin inventory for DP
    for(auto pr: coin_inventory){
        for(int i = 0; i < pr.second; i++){
            coins.push_back(pr.first);
        }
    }

    std::vector<int> dp(diff + 1, 10000000), last_coin(diff + 1, -1);
    dp[0] = 0;

    for(int coin: coins){
        // Iterate backwards to ensure each coin is used once
        for(int i = diff; i - coin >= 0; i--){
            if(dp[i] > dp[i - coin] + 1){
                dp[i] = dp[i - coin] + 1;
                last_coin[i] = coin;
            }
        }
    }

    // If the target amount is still at the initialized state, exact change is impossible
    if(last_coin[diff] == -1){
        return std::nullopt;
    }

    // Backtrack through DP table to determine which coins were used
    int cur = diff;
    while(cur != 0){
        change[last_coin[cur]]++;
        cur -= last_coin[cur];
    }

    return change;
}

bool TicketMachine::pay(int reservationId, int ticket_price, int amount_paid, const UserData& user){
    auto change_result = calculate_change(amount_paid, ticket_price);

    // Cancel reservation if machine lacks correct change
    if(!change_result.has_value()){
        server.cancelReservation(reservationId);
        return false; 
    }

    bool server_success = server.confirmPurchase(reservationId, user);

    if(server_success){
        // Deduct dispensed change from machine's inventory
        std::map<int, int> change_to_give = change_result.value();
        for(auto pr: change_to_give){
            coin_inventory[pr.first] -= pr.second;
        }
        return true;
    } 

    server.cancelReservation(reservationId);
    return false;
}

void TicketMachine::cancel_transaction(int reservationId) {
    server.cancelReservation(reservationId);
}