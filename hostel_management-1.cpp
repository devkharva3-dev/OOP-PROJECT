// Hostel Management System - C++ OOP Project
// Concepts used: Class/Object, Encapsulation, Inheritance, Abstract class,
// Polymorphism (virtual function), Constructor/Destructor, static member, STL vector

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

// ---------- Abstract Base Class ----------
class Person {
protected:
    string name;
    string phone;
public:
    Person(string n = "", string p = "") : name(n), phone(p) {}
    virtual ~Person() {}
    virtual void display() const = 0;   // pure virtual -> abstract class
    string getName() const { return name; }
};

// ---------- Derived Class: Student ----------
class Student : public Person {
    int id;
    string course;
    int roomNo;          // -1 means no room allotted
    double feesPaid;
public:
    static const double TOTAL_FEES;

    Student(int i, string n, string p, string c)
        : Person(n, p), id(i), course(c), roomNo(-1), feesPaid(0) {}

    int getId() const { return id; }
    int getRoomNo() const { return roomNo; }
    void setRoomNo(int r) { roomNo = r; }
    double getFeesPaid() const { return feesPaid; }
    double getDue() const { return TOTAL_FEES - feesPaid; }

    bool payFees(double amt) {
        if (amt <= 0 || amt > getDue()) return false;
        feesPaid += amt;
        return true;
    }

    void display() const override {
        cout << left << setw(6) << id << setw(18) << name << setw(14) << phone
             << setw(10) << course << setw(8)
             << (roomNo == -1 ? string("None") : to_string(roomNo))
             << setw(10) << feesPaid << getDue() << endl;
    }
};
const double Student::TOTAL_FEES = 50000.0;

// ---------- Derived Class: Warden ----------
class Warden : public Person {
    int wardenId;
public:
    Warden(int id, string n, string p) : Person(n, p), wardenId(id) {}
    void display() const override {
        cout << "Warden ID : " << wardenId << "\nName      : " << name
             << "\nPhone     : " << phone << endl;
    }
};

// ---------- Room Class ----------
class Room {
    int roomNo;
    int capacity;
    vector<int> occupants;   // student IDs
public:
    Room(int no, int cap) : roomNo(no), capacity(cap) {}

    int getRoomNo() const { return roomNo; }
    bool isFull() const { return (int)occupants.size() >= capacity; }

    bool addStudent(int sid) {
        if (isFull()) return false;
        occupants.push_back(sid);
        return true;
    }

    bool removeStudent(int sid) {
        for (size_t i = 0; i < occupants.size(); i++) {
            if (occupants[i] == sid) {
                occupants.erase(occupants.begin() + i);
                return true;
            }
        }
        return false;
    }

    void display() const {
        cout << left << setw(8) << roomNo << setw(10) << capacity
             << setw(10) << occupants.size() << "IDs: ";
        if (occupants.empty()) cout << "-";
        for (int id : occupants) cout << id << " ";
        cout << endl;
    }
};

// ---------- Hostel Class (manages everything) ----------
class Hostel {
    string hostelName;
    Warden warden;
    vector<Student> students;
    vector<Room> rooms;

    Student* findStudent(int id) {
        for (auto &s : students) if (s.getId() == id) return &s;
        return nullptr;
    }
    Room* findRoom(int no) {
        for (auto &r : rooms) if (r.getRoomNo() == no) return &r;
        return nullptr;
    }
    static void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    Hostel(string name, Warden w, int totalRooms, int capacity)
        : hostelName(name), warden(w) {
        for (int i = 1; i <= totalRooms; i++) rooms.push_back(Room(100 + i, capacity));
    }

    void showInfo() const {
        cout << "\n===== " << hostelName << " =====\n";
        warden.display();
    }

    void addStudent() {
        int id; string n, p, c;
        cout << "Student ID: "; cin >> id;
        if (findStudent(id)) { cout << "ID already exists!\n"; return; }
        clearInput();
        cout << "Name: "; getline(cin, n);
        cout << "Phone: "; getline(cin, p);
        cout << "Course/Branch: "; getline(cin, c);
        students.push_back(Student(id, n, p, c));
        cout << "Student added successfully.\n";
    }

    void allocateRoom() {
        int id, rno;
        cout << "Student ID: "; cin >> id;
        Student* s = findStudent(id);
        if (!s) { cout << "Student not found!\n"; return; }
        if (s->getRoomNo() != -1) { cout << "Room already allotted: " << s->getRoomNo() << endl; return; }
        cout << "Room No: "; cin >> rno;
        Room* r = findRoom(rno);
        if (!r) { cout << "Room not found!\n"; return; }
        if (!r->addStudent(id)) { cout << "Room is full!\n"; return; }
        s->setRoomNo(rno);
        cout << "Room " << rno << " allotted to " << s->getName() << ".\n";
    }

    void vacateRoom() {
        int id;
        cout << "Student ID: "; cin >> id;
        Student* s = findStudent(id);
        if (!s) { cout << "Student not found!\n"; return; }
        if (s->getRoomNo() == -1) { cout << "No room allotted.\n"; return; }
        findRoom(s->getRoomNo())->removeStudent(id);
        s->setRoomNo(-1);
        cout << "Room vacated.\n";
    }

    void payFees() {
        int id; double amt;
        cout << "Student ID: "; cin >> id;
        Student* s = findStudent(id);
        if (!s) { cout << "Student not found!\n"; return; }
        cout << "Due amount: " << s->getDue() << "\nEnter amount to pay: "; cin >> amt;
        if (s->payFees(amt)) cout << "Payment successful. Remaining due: " << s->getDue() << endl;
        else cout << "Invalid amount!\n";
    }

    void searchStudent() {
        int id;
        cout << "Student ID: "; cin >> id;
        Student* s = findStudent(id);
        if (!s) { cout << "Student not found!\n"; return; }
        printStudentHeader();
        s->display();
    }

    void removeStudent() {
        int id;
        cout << "Student ID: "; cin >> id;
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getId() == id) {
                if (students[i].getRoomNo() != -1)
                    findRoom(students[i].getRoomNo())->removeStudent(id);
                students.erase(students.begin() + i);
                cout << "Student removed.\n";
                return;
            }
        }
        cout << "Student not found!\n";
    }

    void printStudentHeader() const {
        cout << left << setw(6) << "ID" << setw(18) << "Name" << setw(14) << "Phone"
             << setw(10) << "Course" << setw(8) << "Room" << setw(10) << "Paid" << "Due\n";
        cout << string(76, '-') << endl;
    }

    void displayStudents() const {
        if (students.empty()) { cout << "No students yet.\n"; return; }
        printStudentHeader();
        for (const auto &s : students) s.display();   // polymorphic display()
    }

    void displayRooms() const {
        cout << left << setw(8) << "Room" << setw(10) << "Capacity" << setw(10) << "Filled" << "Occupants\n";
        cout << string(50, '-') << endl;
        for (const auto &r : rooms) r.display();
    }
};

// ---------- main ----------
int main() {
    Hostel hostel("BVM Boys Hostel", Warden(1, "Mr. Patel", "9876543210"), 5, 3);
    int ch;

    do {
        cout << "\n====== HOSTEL MANAGEMENT SYSTEM ======\n"
             << "1. Add Student\n"
             << "2. Allocate Room\n"
             << "3. Vacate Room\n"
             << "4. Pay Fees\n"
             << "5. Search Student\n"
             << "6. Display All Students\n"
             << "7. Display Rooms\n"
             << "8. Remove Student\n"
             << "9. Hostel / Warden Info\n"
             << "0. Exit\n"
             << "Enter choice: ";
        if (!(cin >> ch)) { cin.clear(); cin.ignore(1000, '\n'); ch = -1; }

        switch (ch) {
            case 1: hostel.addStudent(); break;
            case 2: hostel.allocateRoom(); break;
            case 3: hostel.vacateRoom(); break;
            case 4: hostel.payFees(); break;
            case 5: hostel.searchStudent(); break;
            case 6: hostel.displayStudents(); break;
            case 7: hostel.displayRooms(); break;
            case 8: hostel.removeStudent(); break;
            case 9: hostel.showInfo(); break;
            case 0: cout << "Thank you!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);

    return 0;
}
