// Lab Sheet-10: Smart Campus Lost & Found Management System
// Simple menu-driven C++ project using structures, vectors, functions and file handling.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

struct Item {
    int id;
    string type;        // Lost or Found
    string itemName;
    string category;
    string description;
    string location;
    string date;        // YYYY-MM-DD
    string studentName;
    string contact;
};

vector<Item> records;
const string DATA_FILE = "lost_found_records.txt";

string lowerText(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return value;
}

void loadRecords() {
    records.clear();
    ifstream file(DATA_FILE);
    Item item;
    while (file >> item.id) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(file, item.type);
        getline(file, item.itemName);
        getline(file, item.category);
        getline(file, item.description);
        getline(file, item.location);
        getline(file, item.date);
        getline(file, item.studentName);
        getline(file, item.contact);
        if (file) records.push_back(item);
    }
    file.close();
}

void saveRecords() {
    ofstream file(DATA_FILE);
    for (const Item &item : records) {
        file << item.id << '\n' << item.type << '\n' << item.itemName << '\n'
             << item.category << '\n' << item.description << '\n'
             << item.location << '\n' << item.date << '\n'
             << item.studentName << '\n' << item.contact << '\n';
    }
    file.close();
}

int nextId() {
    int maximum = 0;
    for (const Item &item : records)
        if (item.id > maximum) maximum = item.id;
    return maximum + 1;
}

// Basic student login for this classroom demonstration.
bool studentLogin() {
    string name, studentId;
    cout << "\n--- Student Login ---\n";
    cout << "Enter student name: ";
    cin >> ws;
    getline(cin, name);
    cout << "Enter student ID: ";
    getline(cin, studentId);

    if (name.empty() || studentId.empty()) {
        cout << "Name and student ID cannot be empty.\n";
        return false;
    }
    cout << "Login successful. Welcome, " << name << "!\n";
    return true;
}

void reportItem(const string &type) {
    Item item;
    item.id = nextId();
    item.type = type;

    cout << "\n--- Report " << type << " Item ---\n";
    cout << "Item name: ";
    cin >> ws;
    getline(cin, item.itemName);
    cout << "Category (ID Card/Wallet/Keys/Electronics/Other): ";
    getline(cin, item.category);
    cout << "Description / identifying details: ";
    getline(cin, item.description);
    cout << "Location (where lost/found): ";
    getline(cin, item.location);
    cout << "Date (YYYY-MM-DD): ";
    getline(cin, item.date);
    cout << "Your name: ";
    getline(cin, item.studentName);
    cout << "Contact information: ";
    getline(cin, item.contact);

    records.push_back(item);
    saveRecords();
    cout << type << " item report saved. Record ID: " << item.id << '\n';
}

void printItem(const Item &item) {
    cout << "\nRecord ID: " << item.id
         << "\nStatus: " << item.type
         << "\nItem: " << item.itemName
         << "\nCategory: " << item.category
         << "\nDescription: " << item.description
         << "\nLocation: " << item.location
         << "\nDate: " << item.date
         << "\nReported by: " << item.studentName
         << "\nContact: " << item.contact << '\n';
}

void viewAllRecords() {
    cout << "\n--- All Lost & Found Records ---\n";
    if (records.empty()) {
        cout << "No records available.\n";
        return;
    }
    for (const Item &item : records) printItem(item);
}

void searchItem() {
    string keyword;
    cout << "Enter item name, category, or location to search: ";
    cin >> ws;
    getline(cin, keyword);
    keyword = lowerText(keyword);

    bool found = false;
    for (const Item &item : records) {
        if (lowerText(item.itemName).find(keyword) != string::npos ||
            lowerText(item.category).find(keyword) != string::npos ||
            lowerText(item.location).find(keyword) != string::npos ||
            lowerText(item.description).find(keyword) != string::npos) {
            printItem(item);
            found = true;
        }
    }
    if (!found) cout << "No matching item found.\n";
}

// Simple automatic matching: compare item name or category, ignoring letter case.
void autoMatch() {
    cout << "\n--- Automatic Lost & Found Matching ---\n";
    bool matchFound = false;

    for (const Item &lost : records) {
        if (lowerText(lost.type) != "lost") continue;

        for (const Item &found : records) {
            if (lowerText(found.type) != "found") continue;

            bool sameName = lowerText(lost.itemName) == lowerText(found.itemName);
            bool sameCategory = lowerText(lost.category) == lowerText(found.category);

            if (sameName || (sameCategory &&
                lowerText(lost.description).find(lowerText(found.description)) != string::npos &&
                !found.description.empty())) {
                cout << "\nPossible match:\n";
                cout << "Lost record ID: " << lost.id << " | Item: " << lost.itemName
                     << " | Location: " << lost.location << '\n';
                cout << "Found record ID: " << found.id << " | Item: " << found.itemName
                     << " | Location: " << found.location << '\n';
                cout << "Please verify the details with the student. This is only a possible match.\n";
                matchFound = true;
            }
        }
    }
    if (!matchFound) cout << "No possible matches found yet.\n";
}

void deleteRecord() {
    int id;
    cout << "Enter record ID to delete: ";
    cin >> id;

    auto position = find_if(records.begin(), records.end(),
        [id](const Item &item) { return item.id == id; });

    if (position == records.end()) {
        cout << "Record ID not found.\n";
        return;
    }

    records.erase(position);
    saveRecords();
    cout << "Record deleted successfully.\n";
}

void showStatistics() {
    int lostCount = 0, foundCount = 0;
    for (const Item &item : records) {
        if (lowerText(item.type) == "lost") lostCount++;
        else if (lowerText(item.type) == "found") foundCount++;
    }
    cout << "\n--- Item Statistics ---\n";
    cout << "Total records: " << records.size() << '\n';
    cout << "Lost items: " << lostCount << '\n';
    cout << "Found items: " << foundCount << '\n';
}

void exportReport() {
    ofstream file("lost_found_report.txt");
    if (!file) {
        cout << "Could not create report file.\n";
        return;
    }

    file << "SMART CAMPUS LOST & FOUND REPORT\n";
    file << "================================\n";
    for (const Item &item : records) {
        file << "\nRecord ID: " << item.id
             << "\nStatus: " << item.type
             << "\nItem: " << item.itemName
             << "\nCategory: " << item.category
             << "\nDescription: " << item.description
             << "\nLocation: " << item.location
             << "\nDate: " << item.date
             << "\nReported by: " << item.studentName
             << "\nContact: " << item.contact << "\n";
    }
    file.close();
    cout << "Report exported to lost_found_report.txt\n";
}

void showMenu() {
    int choice;
    do {
        cout << "\n========== SMART CAMPUS LOST & FOUND ==========\n";
        cout << "1. Report Lost Item\n";
        cout << "2. Report Found Item\n";
        cout << "3. Search Item\n";
        cout << "4. Auto Match Lost & Found\n";
        cout << "5. View All Records\n";
        cout << "6. Delete Record\n";
        cout << "7. Item Statistics\n";
        cout << "8. Export Report\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: reportItem("Lost"); break;
            case 2: reportItem("Found"); break;
            case 3: searchItem(); break;
            case 4: autoMatch(); break;
            case 5: viewAllRecords(); break;
            case 6: deleteRecord(); break;
            case 7: showStatistics(); break;
            case 8: exportReport(); break;
            case 9: cout << "Thank you for using the system.\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);
}

int main() {
    cout << "=============================================\n";
    cout << " SMART CAMPUS LOST & FOUND MANAGEMENT SYSTEM\n";
    cout << "=============================================\n";

    if (studentLogin()) {
        loadRecords();
        showMenu();
    } else {
        cout << "Login failed. Program closed.\n";
    }
    return 0;
}
