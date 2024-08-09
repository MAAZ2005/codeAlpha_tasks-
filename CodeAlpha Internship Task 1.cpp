#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h> 

using namespace std;

const int MAX_COURSES = 100;
float convertLetterGradeToGPA(const string& grade) {
    if (grade == "A+" || grade == "A") return 4.0;
    else if (grade == "A-") return 3.67;
    else if (grade == "B+") return 3.33;
    else if (grade == "B") return 3.00;
    else if (grade == "B-") return 2.67;
    else if (grade == "C+") return 2.33;
    else if (grade == "C") return 2.00;
    else if (grade == "C-") return 1.67;
    else if (grade == "D+") return 1.33;
    else if (grade == "D") return 1.00;
    else if (grade == "F") return 0.0;
    else return 0.0;
}
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void clearScreen() {
    system("CLS");
}

class Course {
public:
    string courseCode;
    string name;
    int creditHours;
    float grade;
    bool taken;

    Course() : courseCode(""), name(""), creditHours(0), grade(0.0), taken(false) {}

    void input() {
        setColor(14); 
        string letterGrade;
        cout << "\n\n\t\tEnter course code: \n";
        cin >> courseCode;
        cout << "\n\n\t\tEnter course name: \n";
        cin.ignore();
        getline(cin, name);
       cout <<  "\n\n\t\tEnter credit hours: \n";
        cin >> creditHours;
        cout << "\n\n\t\tEnter grade (e.g., A+, A, B-): \n";
        cin >> letterGrade;
        grade = convertLetterGradeToGPA(letterGrade);
        taken = true;
    }
};

class Semester {
public:
    Course* courses;
    int numCourses;

    Semester(int numCourses) : numCourses(numCourses) {
        courses = new Course[numCourses];
    }

    ~Semester() {
        delete[] courses;
    }

    void inputCourses() {
        for (int i = 0; i < numCourses; ++i) {
            cout << "\n\n\t\tEnter details for course " + to_string(i + 1) + ":";
            courses[i].input();
        }
        cout << "Courses Added Succesfully \n";
    }

    float calculateGPA() {
        float totalGradePoints = 0;
        int totalCreditHours = 0;
        for (int i = 0; i < numCourses; ++i) {
            if (courses[i].taken) {
                totalGradePoints += courses[i].grade * courses[i].creditHours;
                totalCreditHours += courses[i].creditHours;
            }
        }
        return totalCreditHours ? totalGradePoints / totalCreditHours : 0;
    }

    void displayCourses() {
        setColor(9); 
        for (int i = 0; i < numCourses; ++i) {
            if (courses[i].taken) {
                cout << "\n\n\t\tCourse Code: " + courses[i].courseCode + 
                           ", Name: " + courses[i].name + 
                           ", Credit Hours: " + to_string(courses[i].creditHours) + 
                           ", Grade: " + to_string(courses[i].grade);
            }
        }
    }

    void displaySemesterDetails() {
        setColor(9); 
        cout << "\n\n\t\tSemester Details:\n";
        displayCourses();
    }

    Course* getCourseByCode(const string& code) {
        for (int i = 0; i < numCourses; ++i) {
            if (courses[i].courseCode == code && courses[i].taken) {
                return &courses[i];
            }
        }
        return NULL;
    }
};
class Student {
private:
    Semester** semesters;
    int numSemesters;

public:
    Student() : numSemesters(0), semesters(0) {}

    ~Student() {
        for (int i = 0; i < numSemesters; ++i) {
            delete semesters[i];
        }
        delete[] semesters;
    }

    void addNewSemester() {
        int numCourses;
        setColor(14); 
        cout << "\n\n\t\tEnter the number of courses for the new semester: \n";
        cin >> numCourses;

        Semester** newSemesters = new Semester*[numSemesters + 1];
        for (int i = 0; i < numSemesters; ++i) {
            newSemesters[i] = semesters[i];
        }
        newSemesters[numSemesters] = new Semester(numCourses);
        newSemesters[numSemesters]->inputCourses();

        delete[] semesters;
        semesters = newSemesters;
        numSemesters++;
    }

    void displaySemesters() {
        for (int i = 0; i < numSemesters; ++i) {
            cout << "\n\n\t\tSemester " + to_string(i + 1) + " Details:\n";
            semesters[i]->displaySemesterDetails();
        }
    }

    void checkCourseDetails() {
        string code;
        setColor(14); 
        cout << "\n\n\t\tEnter course code to check details: \n";
        cin >> code;
        for (int i = 0; i < numSemesters; ++i) {
            Course* course = semesters[i]->getCourseByCode(code);
            if (course) {
                setColor(9); 
                cout << "\n\n\t\tCourse Code: " + course->courseCode + 
                           ", Name: " + course->name + 
                           ", Credit Hours: " + to_string(course->creditHours) + 
                           ", Grade: " + to_string(course->grade) + 
                           ", Semester: " + to_string(i + 1);
                return;
            }
        }
        cout << "\n\n\t\tCourse not found.\n";
    }

    void checkSemesterDetails() {
        int semesterNumber;
        setColor(14); 
        cout << "\n\n\t\tEnter semester number to check details: \n";
        cin >> semesterNumber;
        if (semesterNumber < 1 || semesterNumber > numSemesters) {
            cout << "\n\n\t\tInvalid semester number.\n";
            return;
        }
        semesters[semesterNumber - 1]->displaySemesterDetails();
    }

    void calculateAndDisplaySGPA() {
        int semesterNumber;
        setColor(14); 
        cout << "\n\n\t\tEnter semester number to calculate SGPA: \n";
        cin >> semesterNumber;
        if (semesterNumber < 1 || semesterNumber > numSemesters) {
            cout << "\n\n\t\tInvalid semester number.\n";
            return;
        }
        float sgpa = semesters[semesterNumber - 1]->calculateGPA();
        setColor(9); 
        cout <<"\n\n\t\tSGPA for semester " + to_string(semesterNumber) + ": " 
                   + to_string(sgpa) + "\n";
    }

    float calculateCGPA() {
        float totalGradePoints = 0;
        int totalCreditHours = 0;
        for (int i = 0; i < numSemesters; ++i) {
            for (int j = 0; j < semesters[i]->numCourses; ++j) {
                if (semesters[i]->courses[j].taken) {
                    totalGradePoints += semesters[i]->courses[j].grade * semesters[i]->courses[j].creditHours;
                    totalCreditHours += semesters[i]->courses[j].creditHours;
                }
            }
        }
        return totalCreditHours ? totalGradePoints / totalCreditHours : 0;
    }
};

int main() {
    Student student;
    setColor(10); 
     cout << "\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t- - - - GPA CALCULATOR - - - - -\n\n\n\n\n\n\n";
    cout << "\n\n\n";
    system("pause");
    clearScreen();

    int choice;
    do {
        setColor(12);
        cout << "\n\n\n";
        cout << "\n\n\n\n\t\t\t\t Menu:\n";
        cout << "\t\t\t1. Add New Semester\n";
        cout << "\t\t\t2. Display Course Details\n";
        cout << "\t\t\t3. Display Semester Details\n";
        cout << "\t\t\t4. Calculate SGPA\n";
        cout << "\t\t\t5. Calculate CGPA\n";
        cout << "\t\t\t6. Exit\n";
        cout << "\t\tEnter your choice: \n";
        cin >> choice;
        clearScreen();

        switch (choice) {
            case 1:
                student.addNewSemester();
                break;
            case 2:
                student.checkCourseDetails();
                break;
            case 3:
                student.checkSemesterDetails();
                break;
            case 4:
                student.calculateAndDisplaySGPA();
                break;
            case 5:
                setColor(9); 
                cout << "\nCGPA: " 
                           + to_string(student.calculateCGPA());
                break;
            case 6:
                setColor(12); 
                cout << "Exiting program.\n";
                break;
            default:
                setColor(12); 
                cout << "Invalid choice. Please try again.\n";
        }
        cout << "\n\n";
        system("pause");
        clearScreen();
    } while (choice != 6);

    return 0;
}
