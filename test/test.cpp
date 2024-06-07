#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <fstream>
#include <limits>

struct Student
{
    std::string name;
    int id;
    std::string department;
    std::string major;
    std::vector<int> scores;
    int totalScore;

    void calculateTotalScore()
    {
        totalScore = std::accumulate(scores.begin(), scores.end(), 0);
    }
};

std::vector<Student> students;

bool isDuplicateName(const std::string& name);
bool isDuplicateId(int id);
bool addStudent(std::string name, int id, std::string department,
    std::string major, const std::vector<int>& scores);
bool deleteStudentByName(const std::string& name);
bool deleteStudentById(int id);
bool modifyStudentByName(const std::string& name,
    const std::string& department,
    const std::string& major,
    const std::vector<int>& scores);
bool modifyStudentById(int id,
    const std::string& department,
    const std::string& major,
    const std::vector<int>& scores);
Student* findStudentByName(const std::string& name);
Student* findStudentById(int id);

int getTotalScore(const Student* student);
double getAverageScore(const Student* student);

void sortStudentsById();
void sortStudentsByTotalScore(bool ascending = true);
void sortStudentsByScore(int courseIndex, bool ascending = true);

void printStudentTable(const std::vector<Student>& studentsToPrint);
void loadStudentsFromFile(const std::string& filename);
void saveStudentsToFile(const std::string& filename);

void clearScreen()
{
    system("cls");
}

void waitForEnter()
{
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void displayMainMenu()
{
    clearScreen();
    std::cout << "Student Management System\n\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Delete Student\n";
    std::cout << "3. Modify Student\n";
    std::cout << "4. Find Student\n";
    std::cout << "5. Sort Students\n";
    std::cout << "6. Print Student List\n";
    std::cout << "7. Exit\n\n";
    std::cout << "Enter your choice: ";
}

void displayDeleteStudentMenu()
{
    clearScreen();
    std::cout << "Delete Student\n\n";
    std::cout << "1. Delete by Name\n";
    std::cout << "2. Delete by ID\n";
    std::cout << "3. Back to Main Menu\n\n";
    std::cout << "Enter your choice: ";
}

void displayModifyStudentMenu()
{
    clearScreen();
    std::cout << "Modify Student\n\n";
    std::cout << "1. Modify by Name\n";
    std::cout << "2. Modify by ID\n";
    std::cout << "3. Back to Main Menu\n\n";
    std::cout << "Enter your choice: ";
}

void displayFindStudentMenu()
{
    clearScreen();
    std::cout << "Find Student\n\n";
    std::cout << "1. Find by Name\n";
    std::cout << "2. Find by ID\n";
    std::cout << "3. Back to Main Menu\n\n";
    std::cout << "Enter your choice: ";
}

void displaySortStudentsMenu()
{
    clearScreen();
    std::cout << "Sort Students\n\n";
    std::cout << "1. Sort by ID\n";
    std::cout << "2. Sort by Total Score\n";
    std::cout << "3. Sort by Individual Course Score\n";
    std::cout << "4. Back to Main Menu\n\n";
    std::cout << "Enter your choice: ";
}

void displaySortByTotalScoreMenu()
{
    clearScreen();
    std::cout << "Sort by Total Score\n\n";
    std::cout << "1. Ascending\n";
    std::cout << "2. Descending\n";
    std::cout << "3. Back to Main Menu\n\n";
    std::cout << "Enter your choice: ";
}

void displaySortByCourseScoreMenu()
{
    clearScreen();
    std::cout << "Sort by Individual Course Score\n\n";
    std::cout << "1. Ascending\n";
    std::cout << "2. Descending\n";
    std::cout << "3. Back to Main Menu\n\n";
    std::cout << "Enter your choice: ";
}

bool isDuplicateName(const std::string& name)
{
    for (const auto& student : students)
    {
        if (student.name == name)
        {
            return true;
        }
    }
    return false;
}

bool isDuplicateId(int id)
{
    for (const auto& student : students)
    {
        if (student.id == id)
        {
            return true;
        }
    }
    return false;
}

bool addStudent(std::string name, int id, std::string department,
    std::string major, const std::vector<int>& scores)
{

    if (scores.size() != 5)
    {
        std::cerr << "Error: The scores array must contain 5 scores.\n";
        waitForEnter();
        return false;
    }

    if (name.length() > 9 || std::to_string(id).length() > 11 ||
        department.length() > 11 || major.length() > 10)
    {
        std::cerr << "Error: Input length exceeds limit.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        waitForEnter();
        return false;
    }

    for (int score : scores)
    {
        if (std::to_string(score).length() > 5)
        {
            std::cerr << "Error: Each score should be less than or equal to 5 digits.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            waitForEnter();
            return false;
        }
    }

    if (isDuplicateName(name))
    {
        std::cerr << "Error: A student with the name " << name << " already exists.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        waitForEnter();
        return false;
    }

    if (isDuplicateId(id))
    {
        std::cerr << "Error: A student with the ID " << id << " already exists.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        waitForEnter();
        return false;
    }

    students.emplace_back(Student{ std::move(name), id, std::move(department), std::move(major), scores, 0 });
    students.back().calculateTotalScore();
    std::cout << "Student added successfully.\n";
    return true;
}

bool deleteStudentByName(const std::string& name)
{
    auto it = std::find_if(students.begin(), students.end(),
        [&name](const Student& s)
        { return s.name == name; });
    if (it != students.end())
    {
        students.erase(it);
        std::cout << "Student deleted successfully.\n";
        return true;
    }
    else
    {
        std::cerr << "Error: Student with name " << name << " not found.\n";
        waitForEnter();
        return false;
    }
}

bool deleteStudentById(int id)
{
    auto it = std::find_if(students.begin(), students.end(),
        [&id](const Student& s)
        { return s.id == id; });
    if (it != students.end())
    {
        students.erase(it);
        std::cout << "Student deleted successfully.\n";
        return true;
    }
    else
    {
        std::cerr << "Error: Student with ID " << id << " not found.\n";
        waitForEnter();
        return false;
    }
}

bool modifyStudentByName(const std::string& name,
    const std::string& department,
    const std::string& major,
    const std::vector<int>& scores)
{

    Student* student = findStudentByName(name);
    if (student)
    {
        student->department = department;
        student->major = major;
        student->scores = scores;
        student->calculateTotalScore();
        std::cout << "Student modified successfully.\n";
        return true;
    }
    waitForEnter();
    return false;
}

bool modifyStudentById(int id,
    const std::string& department,
    const std::string& major,
    const std::vector<int>& scores)
{

    Student* student = findStudentById(id);
    if (student)
    {
        student->department = department;
        student->major = major;
        student->scores = scores;
        student->calculateTotalScore();
        std::cout << "Student modified successfully.\n";
        return true;
    }
    waitForEnter();
    return false;
}

Student* findStudentByName(const std::string& name)
{
    for (auto& student : students)
    {
        if (student.name == name)
        {
            return &student;
        }
    }
    std::cerr << "Error: Student with name " << name << " not found.\n";
    waitForEnter();
    return nullptr;
}

Student* findStudentById(int id)
{
    for (auto& student : students)
    {
        if (student.id == id)
        {
            return &student;
        }
    }
    std::cerr << "Error: Student with ID " << id << " not found.\n";
    waitForEnter();
    return nullptr;
}

int getTotalScore(const Student* student)
{
    if (student == nullptr)
    {
        std::cerr << "Error: Invalid student pointer.\n";
        waitForEnter();
        return -1;
    }
    return student->totalScore;
}

double getAverageScore(const Student* student)
{
    if (student == nullptr)
    {
        std::cerr << "Error: Invalid student pointer.\n";
        waitForEnter();
        return -1;
    }
    return static_cast<double>(student->totalScore) / student->scores.size();
}

void sortStudentsById()
{
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b)
        { return a.id < b.id; });
    std::cout << "Students sorted by ID.\n";
    printStudentTable(students);
    waitForEnter();
}

void sortStudentsByTotalScore(bool ascending)
{
    std::sort(students.begin(), students.end(),
        [ascending](const Student& a, const Student& b)
        {
            return ascending ? (a.totalScore < b.totalScore) : (a.totalScore > b.totalScore);
        });
    std::cout << "Students sorted by total score ("
        << (ascending ? "ascending" : "descending") << ").\n";
    printStudentTable(students);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    waitForEnter();
}

void sortStudentsByScore(int courseIndex, bool ascending)
{
    courseIndex--;

    if (courseIndex < 0 || courseIndex >= 5)
    {
        std::cerr << "Error: Invalid course index.\n";
        waitForEnter();
        return;
    }

    std::sort(students.begin(), students.end(),
        [courseIndex, ascending](const Student& a, const Student& b)
        {
            return ascending ? (a.scores[courseIndex] < b.scores[courseIndex])
                : (a.scores[courseIndex] > b.scores[courseIndex]);
        });
    std::cout << "Students sorted by course " << courseIndex + 1 << " score ("
        << (ascending ? "ascending" : "descending") << "):\n";
    printStudentTable(students);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    waitForEnter();
}

void printStudentTable(const std::vector<Student>& studentsToPrint)
{
    constexpr int nameWidth = 10;
    constexpr int idWidth = 12;
    constexpr int deptWidth = 12;
    constexpr int majorWidth = 10;
    constexpr int scoreWidth = 6;

    std::cout << std::left << std::setw(nameWidth) << "Name"
        << std::setw(idWidth) << "ID"
        << std::setw(deptWidth) << "Department"
        << std::setw(majorWidth) << "Major"
        << std::setw(scoreWidth) << "Score1 "
        << std::setw(scoreWidth) << "Score2 "
        << std::setw(scoreWidth) << "Score3 "
        << std::setw(scoreWidth) << "Score4 "
        << std::setw(scoreWidth) << "Score5 "
        << std::setw(scoreWidth) << "Total" << std::endl;

    std::cout << std::string(nameWidth + idWidth + deptWidth + majorWidth +
        6 * scoreWidth + 4,
        '-')
        << std::endl;

    for (const auto& student : studentsToPrint)
    {
        std::cout << std::left << std::setw(nameWidth) << student.name
            << std::setw(idWidth) << student.id
            << std::setw(deptWidth) << student.department
            << std::setw(majorWidth) << student.major;
        for (int score : student.scores)
        {
            std::cout << std::setw(scoreWidth + 1) << score;
        }
        std::cout << std::setw(scoreWidth) << student.totalScore << std::endl;
    }
}

void loadStudentsFromFile(const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile)
    {
        std::cerr << "Error: Unable to open file " << filename << " for reading.\n";
        waitForEnter();
        return;
    }

    students.clear();

    Student student;
    while (inFile >> student.name >> student.id >> student.department >> student.major)
    {
        student.scores.resize(5);
        for (int i = 0; i < 5; ++i)
        {
            inFile >> student.scores[i];
        }
        student.calculateTotalScore();
        students.push_back(student);
    }

    inFile.close();
    std::cout << "Data loaded from " << filename << ".\n";
    waitForEnter();
}

void saveStudentsToFile(const std::string& filename)
{
    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "Error: Unable to open file " << filename << " for writing.\n";
        waitForEnter();
        return;
    }

    for (const auto& student : students)
    {
        outFile << student.name << " " << student.id << " "
            << student.department << " " << student.major << " ";
        for (int score : student.scores)
        {
            outFile << score << " ";
        }
        outFile << "\n";
    }

    outFile.close();
    std::cout << "Data saved to " << filename << ".\n";
    waitForEnter();
}

int main()
{
    clearScreen();
    std::cout << "Welcome to Student Management System\n";
    std::cout << "Please maximize the console window to ensure proper display and prevent formatting issues." << std::endl;

    std::cout << "Loading data from students.dat...\n";
    loadStudentsFromFile("students.dat");

    int choice;
    while (true)
    {
        displayMainMenu();
        if (!(std::cin >> choice))
        {
            clearScreen();
            std::cerr << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            waitForEnter();
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice)
        {
        case 1:
        {
            clearScreen();
            std::string name, department, major;
            int id;
            std::vector<int> scores(5);

            std::cout << "Enter name: ";
            std::getline(std::cin, name);

            std::cout << "Enter ID: ";
            std::cin >> id;
            std::cout << "Enter department: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, department);

            std::cout << "Enter major: ";
            std::getline(std::cin, major);

            std::cout << "Enter scores (5 courses): ";
            for (int i = 0; i < 5; ++i)
            {
                std::cin >> scores[i];
            }

            if (addStudent(name, id, department, major, scores))
            {
                saveStudentsToFile("students.dat");
            }
            break;
        }
        case 2:
        {
            displayDeleteStudentMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            switch (choice)
            {
            case 1:
            {
                std::string name;
                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                if (deleteStudentByName(name))
                {
                    saveStudentsToFile("students.dat");
                }
                break;
            }
            case 2:
            {
                int id;
                std::cout << "Enter ID: ";
                std::cin >> id;
                if (deleteStudentById(id))
                {
                    saveStudentsToFile("students.dat");
                }
                break;
            }
            case 3:
                break;
            default:
                clearScreen();
                std::cout << "Invalid choice.\n";
                waitForEnter();
            }
            break;
        }
        case 3:
        {
            displayModifyStudentMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            switch (choice)
            {
            case 1:
            {
                std::string name, department, major;
                std::vector<int> scores(5);

                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                std::cout << "Enter new department: ";
                std::getline(std::cin, department);
                std::cout << "Enter new major: ";
                std::getline(std::cin, major);
                std::cout << "Enter new scores (5 courses): ";
                for (int i = 0; i < 5; ++i)
                {
                    std::cin >> scores[i];
                }
                if (modifyStudentByName(name, department, major, scores))
                {
                    saveStudentsToFile("students.dat");
                }
                break;
            }
            case 2:
            {
                int id;
                std::string department, major;
                std::vector<int> scores(5);
                std::cout << "Enter ID: ";
                std::cin >> id;
                std::cout << "Enter new department: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, department);
                std::cout << "Enter new major: ";
                std::getline(std::cin, major);
                std::cout << "Enter new scores (5 courses): ";
                for (int i = 0; i < 5; ++i)
                {
                    std::cin >> scores[i];
                }
                if (modifyStudentById(id, department, major, scores))
                {
                    saveStudentsToFile("students.dat");
                }
                break;
            }
            case 3:
                break;
            default:
                clearScreen();
                std::cout << "Invalid choice.\n";
                waitForEnter();
            }
            break;
        }
        case 4:
        {
            displayFindStudentMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            switch (choice)
            {
            case 1:
            {
                std::string name;
                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                Student* student = findStudentByName(name);
                if (student != nullptr)
                {
                    std::cout << "Name: " << student->name << std::endl;
                    std::cout << "ID: " << student->id << std::endl;
                    std::cout << "Department: " << student->department << std::endl;
                    std::cout << "Major: " << student->major << std::endl;
                    std::cout << "Scores: ";
                    for (int score : student->scores)
                    {
                        std::cout << score << " ";
                    }
                    std::cout << std::endl;
                    std::cout << "Total Score: " << student->totalScore << std::endl;
                    std::cout << "Average Score: " << getAverageScore(student) << std::endl;
                }
                waitForEnter();
                break;
            }
            case 2:
            {
                int id;
                std::cout << "Enter ID: ";
                std::cin >> id;
                Student* student = findStudentById(id);
                if (student != nullptr)
                {
                    std::cout << "Name: " << student->name << std::endl;
                    std::cout << "ID: " << student->id << std::endl;
                    std::cout << "Department: " << student->department << std::endl;
                    std::cout << "Major: " << student->major << std::endl;
                    std::cout << "Scores: ";
                    for (int score : student->scores)
                    {
                        std::cout << score << " ";
                    }
                    std::cout << std::endl;
                    std::cout << "Total Score: " << student->totalScore << std::endl;
                    std::cout << "Average Score: " << getAverageScore(student) << std::endl;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                waitForEnter();
                break;
            }
            case 3:
                break;
            default:
                clearScreen();
                std::cout << "Invalid choice.\n";
                waitForEnter();
            }
            break;
        }

        case 5:
        {
            displaySortStudentsMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            switch (choice)
            {
            case 1:
                sortStudentsById();
                break;
            case 2:
            {
                displaySortByTotalScoreMenu();
                std::cin >> choice;
                clearScreen();
                switch (choice)
                {
                case 1:
                    sortStudentsByTotalScore(true);
                    break;
                case 2:
                    sortStudentsByTotalScore(false);
                    break;
                case 3:
                    break;
                default:
                    clearScreen();
                    std::cout << "Invalid choice.\n";
                    waitForEnter();
                }
                break;
            }
            case 3:
            {
                displaySortByCourseScoreMenu();
                std::cin >> choice;
                int courseIndex;
                if (choice == 3)
                {
                    break;
                }

                clearScreen();
                std::cout << "Enter course index (1-5): ";
                std::cin >> courseIndex;

                switch (choice)
                {
                case 1:
                    sortStudentsByScore(courseIndex, true);
                    break;
                case 2:
                    sortStudentsByScore(courseIndex, false);
                    break;
                case 3:
                    break;
                default:
                    clearScreen();
                    std::cout << "Invalid choice.\n";
                    waitForEnter();
                }
                break;
            }
            case 4:
                break;
            default:
                clearScreen();
                std::cout << "Invalid choice.\n";
                waitForEnter();
            }
            break;
        }
        case 6:
            clearScreen();
            printStudentTable(students);
            waitForEnter();
            break;
        case 7:
            clearScreen();
            std::cout << "Exiting program...\n";
            saveStudentsToFile("students.dat");
            clearScreen();
            return 0;
        default:
            clearScreen();
            std::cout << "Invalid choice.\n";
            waitForEnter();
        }
    }
    return 0;
}