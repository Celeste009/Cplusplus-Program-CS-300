#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// Structure to hold course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Node structure for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BinarySearchTree {
private:
    Node* root;

    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        } else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        } else {
            insert(node->right, course);
        }
    }

    Node* search(Node* node, const string& courseNumber) const {
        if (node == nullptr || node->course.courseNumber == courseNumber) {
            return node;
        }
        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        } else {
            return search(node->right, courseNumber);
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseName << endl;
            inOrder(node->right);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(Course course) {
        insert(root, course);
    }

    Node* search(const string& courseNumber) const {
        return search(root, courseNumber);
    }

    void inOrder() const {
        inOrder(root);
    }
};

// Helper function to convert a string to uppercase
string toUpperCase(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        }
    }
    return str;
}

// Function to load courses from a CSV file, check for errors, and store them in a map
map<string, Course> loadCoursesFromFile(const string& filename) {
    map<string, Course> coursesMap;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, courseName, prereq;
        vector<string> prerequisites;
        
        getline(ss, courseNumber, ',');
        getline(ss, courseName, ',');

        while (getline(ss, prereq, ',')) {
            prerequisites.push_back(prereq);
        }

        if (courseNumber.empty() || courseName.empty()) {
            cout << "Error: Invalid format in line" << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(courseNumber);  // Convert to uppercase
        course.courseName = courseName;
        course.prerequisites = prerequisites;

        coursesMap[course.courseNumber] = course;
    }

    // Validate prerequisites
    for (const auto& pair : coursesMap) {
        for (const string& prereq : pair.second.prerequisites) {
            if (coursesMap.find(prereq) == coursesMap.end()) {
                cout << "Error: Prerequisite " << prereq << " for course " << pair.first << " does not exist." << endl;
            }
        }
    }

    return coursesMap;
}

// Function to create course objects and store them in a Binary Search Tree
BinarySearchTree createCourseObjectsAndStoreInTree(const map<string, Course>& coursesMap) {
    BinarySearchTree coursesTree;

    for (const auto& entry : coursesMap) {
        coursesTree.insert(entry.second);
    }

    return coursesTree;
}

// Function to print course information and prerequisites
void printCourseInformation(const BinarySearchTree& coursesTree, string courseNumber) {
    courseNumber = toUpperCase(courseNumber);  // Convert input to uppercase
    Node* courseNode = coursesTree.search(courseNumber);

    if (courseNode != nullptr) {
        cout << courseNode->course.courseNumber << ", " << courseNode->course.courseName << endl;
        cout << "Prerequisites: ";
        if (courseNode->course.prerequisites.empty()) {
            cout << "None" << endl;
        } else {
            for (const string& prereq : courseNode->course.prerequisites) {
                Node* prereqNode = coursesTree.search(prereq);
                if (prereqNode != nullptr) {
                    cout << prereq << " (" << prereqNode->course.courseName << "), ";
                } else {
                    cout << prereq << ", ";
                }
            }
            cout << endl;
        }
    } else {
        cout << "Course not found" << endl;
    }
}

// Main function
int main() {
    map<string, Course> coursesMap;
    BinarySearchTree coursesTree;
    int choice;
    string filename = "CS_300_ABCU_Advising_Program_Input.csv";
    string courseNumber;

    cout << "Welcome to the course planner." << endl;
    do {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
            case 1:
                coursesMap = loadCoursesFromFile(filename);
                coursesTree = createCourseObjectsAndStoreInTree(coursesMap);
                cout << "Data loaded successfully." << endl;
                break;
            case 2:
                cout << "Here is a sample schedule:" << endl;
                coursesTree.inOrder();
                break;
            case 3:
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourseInformation(coursesTree, courseNumber);
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option." << endl;
        }
    } while (choice != 9);

    return 0;
}
