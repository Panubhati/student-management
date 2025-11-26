#include "student.h"

// default constructor
Student::Student() : roll(0), marks(0.0) {}

// parameterized constructor
Student::Student(int r, string n, string c, double m)
    : roll(r), name(n), course(c), marks(m) {}

// print student info
void Student::display() const {
    cout << "Roll: " << roll
         << ", Name: " << name
         << ", Course: " << course
         << ", Marks: " << marks << endl;
}

// getters
int Student::getRoll() const { return roll; }
string Student::getName() const { return name; }
string Student::getCourse() const { return course; }
double Student::getMarks() const { return marks; }

// setters
void Student::setRoll(int r) { roll = r; }
void Student::setName(string n) { name = n; }
void Student::setCourse(string c) { course = c; }
void Student::setMarks(double m) { marks = m; }
