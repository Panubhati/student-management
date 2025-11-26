#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
using namespace std;

// Student class with private data members
class Student {
private:
    int roll;
    string name;
    string course;
    double marks;

public:
    // default constructor
    Student();

    // parameterized constructor
    Student(int r, string n, string c, double m);

    // display student details
    void display() const;

    // getters
    int getRoll() const;
    string getName() const;
    string getCourse() const;
    double getMarks() const;

    // setters
    void setRoll(int r);
    void setName(string n);
    void setCourse(string c);
    void setMarks(double m);
};

#endif
