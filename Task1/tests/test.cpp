#include <gtest/gtest.h>
#include "../src/Server.h"
#include "../src/TicketMachine.h"

// Przykład 1: Pomyślny zakup biletu
TEST(TicketMachineTests, Przyklad1) {
    Server server(10);
    
    // Stan początkowy: 1 x 2.00 zł, 5 x 1.00 zł, 2 x 0.50 zł
    std::map<int, int> coins = {{200, 1}, {100, 5}, {50, 2}};
    TicketMachine machine(server, coins);

    // Wybór: Bilet 3.50 zł
    int ticket_price = 350;
    int res_id = machine.select_ticket(ticket_price);
    ASSERT_NE(res_id, -1);

    // Dane i Płatność: Jan Kowalski, wrzuca 5.00 zł
    UserData user{"Jan Kowalski"};
    bool success = machine.pay(res_id, ticket_price, 500, user);

    // Wynik: Sukces
    EXPECT_TRUE(success);
}

// Przykład 2: Przerwanie procesu zakupu
TEST(TicketMachineTests, Przyklad2) {
    Server server(10);
    std::map<int, int> coins = {{100, 10}};
    TicketMachine machine(server, coins);

    EXPECT_EQ(server.getAvailableTickets(), 10);

    // Wybór: Bilet ulgowy 1.70 zł
    int res_id = machine.select_ticket(170);
    
    // Reakcja serwera: Bilet otrzymuje status tymczasowej rezerwacji
    EXPECT_EQ(server.getAvailableTickets(), 9);

    // Przerwanie: Użytkownik wciska "Anuluj"
    machine.cancel_transaction(res_id);

    // Wynik: Serwer zdejmuje rezerwację, zwracając bilet do puli
    EXPECT_EQ(server.getAvailableTickets(), 10);
}

// Przykład 3: Wybór biletu i brak aktywności (Timeout)
TEST(TicketMachineTests, Przyklad3) {
    Server server(1); // Stan początkowy: Pula wynosi 1
    std::map<int, int> coins = {{100, 10}};
    
    TicketMachine machine1(server, coins, 500);
    TicketMachine machine2(server, coins, 500);

    // Pierwszy użytkownik klika w bilet
    int res1 = machine1.select_ticket(350);
    EXPECT_NE(res1, -1);
    EXPECT_EQ(server.getAvailableTickets(), 0);

    // Drugi automat wyświetla brak biletów
    int res2 = machine2.select_ticket(350);
    EXPECT_EQ(res2, -1);

    // Brak aktywności: Czekamy na timeout
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Wynik: Timeout zwalnia rezerwację i bilet wraca do puli
    EXPECT_EQ(server.getAvailableTickets(), 1); 

    // Drugi użytkownik może teraz kupić
    int res3 = machine2.select_ticket(350);
    EXPECT_NE(res3, -1);
}

// Przykład 4: Brak monet do wydania reszty
TEST(TicketMachineTests, Przyklad4) {
    Server server(10); 
    
    // Stan początkowy: 10 x 2.00 zł
    std::map<int, int> coins = {{200, 10}};
    TicketMachine machine(server, coins);

    // Wybór: Bilet 2.80 zł
    int ticket_price = 280;
    int res_id = machine.select_ticket(ticket_price);
    ASSERT_NE(res_id, -1); 

    // Dane i Płatność: wrzuca 5.00 zł, reszta = 2.20 zł
    UserData user{"Jan Kowalski"};
    bool success = machine.pay(res_id, ticket_price, 500, user);

    // Wynik: Odrzucone z powodu braku reszty
    EXPECT_FALSE(success);
}