#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Utility function to auto-generate unique IDs
int generateID(const string& filename) {
    ifstream file(filename);
    int id = 1000; // Start IDs from 1000
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        ss >> id; // Extract the first value (ID)
    }
    file.close();
    return id + 1;
}

// Passenger Management
class Passenger {
public:
    struct PassengerDetails {
        int id;
        string name;
        string gender;
        int age;
        string phone;
    };

    void addPassengerDetails() {
        PassengerDetails p;
        p.id = generateID("passengers.txt");

        cout << "Enter Passenger Name: ";
        cin.ignore();
        getline(cin, p.name);
        cout << "Enter Passenger Gender: ";
        cin >> p.gender;
        cout << "Enter Passenger Age: ";
        cin >> p.age;
        cout << "Enter Passenger Phone Number: ";
        cin >> p.phone;

        ofstream file("passengers.txt", ios::app);
        file << p.id << "," << p.name << "," << p.gender << "," << p.age << "," << p.phone << endl;
        file.close();

        cout << "Passenger registered successfully! Assigned ID: " << p.id << endl;
    }

    PassengerDetails fetchPassenger(int id) {
        ifstream file("passengers.txt");
        string line;
        PassengerDetails p;

        while (getline(file, line)) {
            stringstream ss(line);
            string temp;
            getline(ss, temp, ',');
            int fetchedID = stoi(temp);

            if (fetchedID == id) {
                p.id = fetchedID;
                getline(ss, p.name, ',');
                getline(ss, p.gender, ',');
                ss >> p.age;
                getline(ss, temp, ','); // Skip comma
                getline(ss, p.phone);
                file.close();
                return p;
            }
        }
        file.close();
        p.id = -1; // Indicate not found
        return p;
    }
};

// Flight Management
struct Flight {
    string source;
    string destination;
    string flightNumber;
    string date;
    string duration;
    int price;
};

class FlightManagement {
public:
    void addFlight() {
        Flight f;
        cout << "Enter Source: ";
        cin >> f.source;
        cout << "Enter Destination: ";
        cin >> f.destination;
        cout << "Enter Flight Number: ";
        cin >> f.flightNumber;
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> f.date;
        cout << "Enter Duration: ";
        cin >> f.duration;
        cout << "Enter Price: ";
        cin >> f.price;

        ofstream file("flights.txt", ios::app);
        file << f.source << "," << f.destination << "," << f.flightNumber << ","
             << f.date << "," << f.duration << "," << f.price << endl;
        file.close();

        cout << "Flight added successfully!\n";
    }

    vector<Flight> loadFlights() {
        vector<Flight> flights;
        ifstream file("flights.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            Flight f;
            getline(ss, f.source, ',');
            getline(ss, f.destination, ',');
            getline(ss, f.flightNumber, ',');
            getline(ss, f.date, ',');
            getline(ss, f.duration, ',');
            ss >> f.price;

            flights.push_back(f);
        }
        file.close();
        return flights;
    }
};

// Ticket Management
class Ticket {
public:
    struct TicketDetails {
        int ticketID;
        int passengerID;
        string flightNumber;
        float price;
    };

    void generateTicket(int passengerID, const Flight& flight) {
        int ticketID = generateID("tickets.txt");

        ofstream file("tickets.txt", ios::app);
        file << ticketID << "," << passengerID << "," << flight.flightNumber << "," << flight.price << endl;
        file.close();

        cout << "Ticket booked successfully! Ticket ID: " << ticketID << endl;
    }
};

// Main Menu
void mainMenu() {
    Passenger p;
    FlightManagement fm;
    Ticket t;

    while (true) {
        int choice;
        cout << "\n--- IIIT Airlines ---\n";
        cout << "1. Add Passenger\n";
        cout << "2. Search Passenger\n";
        cout << "3. Book Flight\n";
        cout << "4. Add Flight\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                p.addPassengerDetails();
                break;

            case 2: {
                int id;
                cout << "Enter Passenger ID: ";
                cin >> id;
                Passenger::PassengerDetails details = p.fetchPassenger(id);
                if (details.id != -1) {
                    cout << "Passenger Found: " << details.name << ", " << details.gender
                         << ", Age: " << details.age << ", Phone: " << details.phone << endl;
                } else {
                    cout << "Passenger not found!\n";
                }
                break;
            }

            case 3: {
                vector<Flight> flights = fm.loadFlights();
                if (flights.empty()) {
                    cout << "No flights available.\n";
                    break;
                }

                cout << "Available Flights:\n";
                for (int i = 0; i < flights.size(); i++) {
                    cout << i + 1 << ". " << flights[i].source << " -> " << flights[i].destination
                         << " | Flight: " << flights[i].flightNumber
                         << " | Date: " << flights[i].date
                         << " | Price: Rs. " << flights[i].price << endl;
                }

                int flightChoice;
                cout << "Select a flight to book (1-" << flights.size() << "): ";
                cin >> flightChoice;

                if (flightChoice < 1 || flightChoice > flights.size()) {
                    cout << "Invalid choice.\n";
                    break;
                }

                cout << "Enter Passenger ID: ";
                int passengerID;
                cin >> passengerID;
                Passenger::PassengerDetails passDetails = p.fetchPassenger(passengerID);

                if (passDetails.id == -1) {
                    cout << "Passenger not found. Please register first.\n";
                    break;
                }

                t.generateTicket(passengerID, flights[flightChoice - 1]);
                break;
            }

            case 4:
                fm.addFlight();
                break;

            case 5:
                cout << "Thank you for choosing IIIT Airlines!\n";
                return;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    mainMenu();
    return 0;
}
