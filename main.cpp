#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// -------------------- Utility (Case Normalizer) --------------------
string normalize(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// -------------------- Flight Class --------------------
class Flight {
private:
    string flightNo, source, destination;
    int totalSeats;
    vector<bool> seats;

public:
    Flight(string fn, string s, string d, int count)
        : flightNo(fn), source(normalize(s)), destination(normalize(d)), totalSeats(count) {
        seats.resize(totalSeats, false);
    }

    string getFlightNo() const { return flightNo; }
    string getSource() const { return source; }
    string getDestination() const { return destination; }

    void display() const {
        cout << "\nFlight: " << flightNo
             << " | From: " << source
             << " | To: " << destination
             << " | Available Seats: " << countAvailable();
    }

    int countAvailable() const {
        int c = 0;
        for (bool s : seats)
            if (!s) c++;
        return c;
    }

    int bookSeat() {
        for (int i = 0; i < totalSeats; i++) {
            if (!seats[i]) {
                seats[i] = true;
                return i + 1;
            }
        }
        return -1;
    }

    bool cancelSeat(int seat) {
        if (seat > 0 && seat <= totalSeats && seats[seat - 1]) {
            seats[seat - 1] = false;
            return true;
        }
        return false;
    }

    bool match(string s, string d) {
        return source == normalize(s) && destination == normalize(d);
    }
};

// -------------------- Ticket Class --------------------
class Ticket {
private:
    static int id;
    int ticketID;
    string flightNo;
    int seatNo;

public:
    Ticket(string f, int s) {
        ticketID = ++id;
        flightNo = f;
        seatNo = s;
    }

    int getID() const { return ticketID; }
    string getFlight() const { return flightNo; }
    int getSeat() const { return seatNo; }

    void show() const {
        cout << "\n----- TICKET -----";
        cout << "\nID     : " << ticketID;
        cout << "\nFlight : " << flightNo;
        cout << "\nSeat   : " << seatNo;
        cout << "\n------------------\n";
    }
};

int Ticket::id = 1000;

// -------------------- Reservation System --------------------
class System {
private:
    vector<Flight> flights;
    vector<Ticket> tickets;

public:
    System() {
        // Domestic + International flights (realistic)
        flights.push_back(Flight("PK101", "Lahore", "Karachi", 5));
        flights.push_back(Flight("PK102", "Islamabad", "Lahore", 4));
        flights.push_back(Flight("PK201", "Karachi", "Dubai", 6));
        flights.push_back(Flight("PK202", "Lahore", "Dubai", 5));
        flights.push_back(Flight("PK301", "Islamabad", "London", 3));
        flights.push_back(Flight("PK302", "Karachi", "Istanbul", 4));
        flights.push_back(Flight("PK303", "Peshawar", "Doha", 5));
    }

    void showFlights() {
        cout << "\n\n===== AVAILABLE FLIGHTS =====";
        for (auto &f : flights)
            f.display();
        cout << "\n";
    }

    Flight* findFlight(string s, string d) {
        for (auto &f : flights)
            if (f.match(s, d))
                return &f;
        return nullptr;
    }

    void searchFlight() {
        string s, d;
        cout << "\nEnter Source: ";
        cin >> s;
        cout << "Enter Destination: ";
        cin >> d;

        Flight* f = findFlight(s, d);
        if (f) f->display();
        else cout << "\nNo flight found!\n";
    }

    void bookFlight() {
        string s, d;
        cout << "\nEnter Source: ";
        cin >> s;
        cout << "Enter Destination: ";
        cin >> d;

        Flight* f = findFlight(s, d);

        if (!f) {
            cout << "\nNo flight available!\n";
            return;
        }

        int seat = f->bookSeat();

        if (seat == -1) {
            cout << "\nFlight Full!\n";
            return;
        }

        Ticket t(f->getFlightNo(), seat);
        tickets.push_back(t);

        cout << "\nBooking Successful!";
        t.show();
    }

    void cancelTicket() {
        int id;
        cout << "\nEnter Ticket ID: ";
        cin >> id;

        for (int i = 0; i < tickets.size(); i++) {
            if (tickets[i].getID() == id) {
                string fno = tickets[i].getFlight();
                int seat = tickets[i].getSeat();

                for (auto &f : flights)
                    if (f.getFlightNo() == fno)
                        f.cancelSeat(seat);

                tickets.erase(tickets.begin() + i);
                cout << "\nTicket Cancelled Successfully!\n";
                return;
            }
        }

        cout << "\nTicket Not Found!\n";
    }

    void showTickets() {
        if (tickets.empty()) {
            cout << "\nNo bookings yet!\n";
            return;
        }

        for (auto &t : tickets)
            t.show();
    }
};

// -------------------- MAIN --------------------
int main() {
    System sys;
    int choice;

    do {
        cout << "\n\n===== AIRLINE RESERVATION SYSTEM =====";
        cout << "\n1. View Flights";
        cout << "\n2. Search Flight";
        cout << "\n3. Book Flight";
        cout << "\n4. Cancel Ticket";
        cout << "\n5. View Tickets";
        cout << "\n0. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: sys.showFlights(); break;
        case 2: sys.searchFlight(); break;
        case 3: sys.bookFlight(); break;
        case 4: sys.cancelTicket(); break;
        case 5: sys.showTickets(); break;
        case 0: cout << "\nExiting...\n"; break;
        default: cout << "\nInvalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}