#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "student.h"
using namespace std;

vector<Student> students;
string CSV_FILE = "students.csv";
string DAT_FILE = "students.dat";

// raw struct for safe binary storage
struct RawStudent {
    int roll;
    char name[50];
    char course[50];
    double marks;
};

// convert string to lowercase
string toLower(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

// split CSV line
vector<string> splitCSV(const string &line) {
    vector<string> result;
    string item;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"')
            inQuotes = !inQuotes;
        else if (c == ',' && !inQuotes) {
            result.push_back(item);
            item.clear();
        } else {
            item.push_back(c);
        }
    }
    result.push_back(item);
    return result;
}

// load from CSV
void loadFromCSV() {
    students.clear();
    ifstream file(CSV_FILE);
    if (!file.is_open()) return;

    string line;
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> f = splitCSV(line);

        if (f.size() < 4) continue;

        Student s(stoi(f[0]), f[1], f[2], stod(f[3]));
        students.push_back(s);
    }

    file.close();
}

// save to CSV
void saveToCSV() {
    ofstream file(CSV_FILE, ios::trunc);
    file << "roll,name,course,marks\n";

    for (auto &s : students) {
        file << s.getRoll() << ","
             << s.getName() << ","
             << s.getCourse() << ","
             << s.getMarks() << "\n";
    }

    file.close();
}

// load from DAT (binary)
void loadFromDAT() {
    ifstream fin(DAT_FILE, ios::binary);
    if (!fin) return;

    students.clear();
    RawStudent rs;

    while (fin.read((char*)&rs, sizeof(rs))) {
        Student s;
        s.setRoll(rs.roll);
        s.setName(rs.name);
        s.setCourse(rs.course);
        s.setMarks(rs.marks);
        students.push_back(s);
    }

    fin.close();
}

// save to DAT using RawStudent struct
void saveToDAT() {
    ofstream fout(DAT_FILE, ios::binary | ios::trunc);

    for (auto &s : students) {
        RawStudent rs;
        rs.roll = s.getRoll();
        strcpy(rs.name, s.getName().c_str());
        strcpy(rs.course, s.getCourse().c_str());
        rs.marks = s.getMarks();

        fout.write((char*)&rs, sizeof(rs));
    }

    fout.close();
}

// save to both formats
void saveAll() {
    saveToCSV();
    saveToDAT();
}

// print header
void header(const string &title) {
    cout << "\n==== " << title << " ====\n\n";
}

// check duplicate roll
bool isDuplicateRoll(int roll, int ignoreIndex = -1) {
    for (int i = 0; i < students.size(); i++) {
        if (i == ignoreIndex) continue;
        if (students[i].getRoll() == roll) return true;
    }
    return false;
}

// add student
void addStudent() {
    header("Add Student");

    int roll;
    while (true) {
        cout << "Enter Roll Number: ";
        cin >> roll;

        if (cin.fail() || roll <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid roll!\n";
        } else if (isDuplicateRoll(roll)) {
            cout << "Roll already exists.\n";
        } else break;
    }

    cin.ignore();
    string name, course;

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Course: ";
    getline(cin, course);

    double marks;
    while (true) {
        cout << "Enter Marks (0–100): ";
        cin >> marks;

        if (cin.fail() || marks < 0 || marks > 100) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid marks!\n";
        } else break;
    }

    Student s(roll, name, course, marks);
    students.push_back(s);
    saveAll();

    cout << "\nStudent added successfully!\n";
}

// view all
void viewStudents() {
    header("All Students");

    cout << left << setw(10) << "Roll"
         << setw(20) << "Name"
         << setw(20) << "Course"
         << setw(10) << "Marks" << "\n";

    for (auto &s : students) {
        cout << setw(10) << s.getRoll()
             << setw(20) << s.getName()
             << setw(20) << s.getCourse()
             << setw(10) << s.getMarks() << "\n";
    }
}

// search by roll
void searchByRoll() {
    header("Search by Roll");

    int roll;
    cout << "Enter Roll: ";
    cin >> roll;

    for (auto &s : students) {
        if (s.getRoll() == roll) {
            s.display();
            return;
        }
    }
    cout << "No student found.\n";
}

// search by name
void searchByName() {
    header("Search by Name");

    cin.ignore();
    string key;
    cout << "Enter name keyword: ";
    getline(cin, key);

    string low = toLower(key);
    bool found = false;

    for (auto &s : students) {
        if (toLower(s.getName()).find(low) != string::npos) {
            s.display();
            found = true;
        }
    }

    if (!found) cout << "No matching student.\n";
}

// sort by name
void sortByName() {
    sort(students.begin(), students.end(),
         [](Student a, Student b) {
             return toLower(a.getName()) < toLower(b.getName());
         });
    saveAll();
}

// sort by marks
void sortByMarks() {
    sort(students.begin(), students.end(),
         [](Student a, Student b) {
             return a.getMarks() > b.getMarks();
         });
    saveAll();
}

// report
void report() {
    header("Report");

    if (students.empty()) {
        cout << "No data.\n";
        return;
    }

    double total = 0, maxMarks = -1;
    string topper;
    int above75 = 0;

    for (auto &s : students) {
        total += s.getMarks();

        if (s.getMarks() > maxMarks) {
            maxMarks = s.getMarks();
            topper = s.getName();
        }

        if (s.getMarks() >= 75) above75++;
    }

    cout << "Topper: " << topper << " (" << maxMarks << ")\n";
    cout << "Average: " << (total / students.size()) << "\n";
    cout << ">=75: " << above75 << "\n";
    cout << "Total: " << students.size() << "\n";
}

// admin login
bool adminLogin() {
    header("Admin Login");

    string pass;
    cout << "Enter password: ";
    cin >> pass;

    return pass == "admin123";
}

// main
int main() {
    loadFromCSV();
    if (students.empty()) loadFromDAT();

    if (!adminLogin()) {
        cout << "Access Denied.\n";
        return 0;
    }

    int choice;

    do {
        header("Student Management System");
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Search by Roll\n";
        cout << "4. Search by Name\n";
        cout << "5. Sort by Name\n";
        cout << "6. Sort by Marks\n";
        cout << "7. Report\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addStudent(); break;
        case 2: viewStudents(); break;
        case 3: searchByRoll(); break;
        case 4: searchByName(); break;
        case 5: sortByName(); break;
        case 6: sortByMarks(); break;
        case 7: report(); break;
        case 0: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
