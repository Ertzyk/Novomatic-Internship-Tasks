#include "Server.h"
#include <thread>
#include <chrono>
#include <mutex>

Server::Server(int initial_tickets) : available_tickets(initial_tickets), next_reservation_id(1) {}

int Server::reserveTicket(){
    // Lock mutex to ensure safe access to ticket inventory
    std::lock_guard<std::mutex> lock(mtx);

    if(available_tickets == 0) return -1;

    available_tickets--;

    int id = next_reservation_id++;
    pending_reservations.insert(id);
    return id;
}

bool Server::confirmPurchase(int reservationId, const UserData& user){
    std::lock_guard<std::mutex> lock(mtx);

    auto it = pending_reservations.find(reservationId);
    if(it == pending_reservations.end() || user.name.empty()) return false;

    // Move reservation from pending to confirmed state
    pending_reservations.erase(it);
    reservations[reservationId] = user;

    return true;
}

void Server::cancelReservation(int reservation_id){
    std::lock_guard<std::mutex> lock(mtx);

    auto it = pending_reservations.find(reservation_id);
    if(it != pending_reservations.end()){
        // Free up the ticket for other users
        pending_reservations.erase(it);
        available_tickets++;
    }
    return;
}

int Server::getAvailableTickets() {
    std::lock_guard<std::mutex> lock(mtx);
    return available_tickets;
}