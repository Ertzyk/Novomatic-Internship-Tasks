#pragma once
#include <string>
#include <mutex>
#include <unordered_map>
#include <set>

struct UserData {
    std::string name;
};

class Server {
private:
    int available_tickets;
    std::unordered_map<int, UserData> reservations;
    std::mutex mtx;
    int next_reservation_id;
    std::set<int> pending_reservations;

public:
    Server(int initial_tickets);

    int reserveTicket(); 

    bool confirmPurchase(int reservationId, const UserData& user);

    void cancelReservation(int reservationId);

    int getAvailableTickets();
};