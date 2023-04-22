//=================================================
//Name:		Project 2
//Author:	Callie Oleson
//Version:	1.0
//Class:	CS300
//Date:		4/12/2023
//=================================================

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//Global definitions available to all methods and classes

//define structure to hold course information
struct Course {
	string courseId;
	string courseName;
	string prereq1;
	string prereq2;
};

//internal structure for tree node
struct Node {
	Course course;
	Node *left;
	Node *right;
	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}
	//initialize with a course
	Node(Course aCourse) :
		Node() {
			course = aCourse;
		}
	
};

//Course Tree Class Definition
class CourseTree {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void alphaSort(Node* node);

public:
	CourseTree();
	void Insert(Course course);
	void AlphaSort();
	Course Search(CourseTree* courseTree, string courseId);
	

};

//default constructor
CourseTree::CourseTree() {
	//initialize housekeeping variables
	root = nullptr;
}

//Insert a course
void CourseTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

//search for a course
Course CourseTree::Search(CourseTree* courseTree, string courseId) {
	Node* currentNode = courseTree->root;
	
	//keep looping downward through tree until bottom reached or match found
	while (currentNode != nullptr) {

		//if match found, return course info
		if (courseId == currentNode->course.courseId) {
			return currentNode->course;
		}
		//if courseId is smaller than current node, traverse left
		else if (courseId < currentNode->course.courseId) {
			currentNode = currentNode->left;
		}
		//else larger, so traverse right
		else {
			currentNode = currentNode->right;
		}
	}
}

//adding course to nodes
void CourseTree::addNode(Node* node, Course course) {
	//if node is larger, add to left
	if (node->course.courseId.compare(course.courseId) > 0) {
		//if no left node, this node becomes left
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			//recurse down left node
			this->addNode(node->left, course);
		}
	}
	else {
		//if no right node, node becomes right
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			//recurse down right node
			this->addNode(node->right, course);
		}
	}
}

//Alphanumeric sort
void CourseTree::AlphaSort() {
	this->alphaSort(root);
}

void CourseTree::alphaSort(Node* node) {
	if (node != nullptr) {
		alphaSort(node->left);
		std::cout << node->course.courseId << ", " << node->course.courseName << endl;
		alphaSort(node->right);
	}
}



//Static methods

void displayCourse(Course course) {
	std::cout << course.courseId << ", " << course.courseName << endl;
	
	if (!course.prereq1.empty()) {
		std::cout << "Prerequisites: " << course.prereq1;
	}
	if (!course.prereq2.empty()) {
		std::cout << ", " << course.prereq2 << endl;
	}
	return;
	
}

void loadCourseList(CourseTree* CourseTree) {
	ifstream inFS;
	Course course;
	string courseInfo;

	std::cout << "Opening Course List..." << endl;
	inFS.open("ABCU_Advising_Program_Input.txt");
	if (!inFS.is_open()) {
		std::cout << "Could not open file";
		return;
	}

	//read first line, get first course id
	std::getline(inFS, course.courseId, ',');
	while (!inFS.fail()) {
		inFS.clear();

		//get line
		std::getline(inFS, courseInfo);
		//add line to string stream and parse info into objects
		std::istringstream ss(courseInfo);
		std::getline(ss, course.courseName, ',');
		std::getline(ss, course.prereq1, ',');
		std::getline(ss, course.prereq2);
		ss.clear();
		inFS.clear();
		
		
		CourseTree->Insert(course);
		//go to next line
		std::getline(inFS, course.courseId, ',');
	}
}

//the one and only main method
int main() {
	//define a tree to hold courses
	CourseTree* courseListTree;
	courseListTree = new CourseTree();
	Course course;

	int userChoice = 0;
	std::cout << "Welcome to the Course Planner" << endl;
	while (userChoice != 9) {
		std::cout << "1. Load Data Structure" << endl;
		std::cout << "2. Print Course List" << endl;
		std::cout << "3. Print Course" << endl;
		std::cout << "9. Exit" << endl;
		std::cout << "What would you like to do?" << endl;
		std::cin >> userChoice;
		if (userChoice > 4 && userChoice < 9) {
			std::cout << userChoice << " is not a valid option" << endl;
		}

		switch (userChoice) {
		case 1:
			loadCourseList(courseListTree);
			break;
		
		case 2:
			courseListTree->AlphaSort();
			break;
			
		case 3:
			string userCourse;
			std::cout << "What course do you want to know about?" << endl;
			std::cin >> userCourse;
			course = courseListTree->Search(courseListTree, userCourse);
			if (!course.courseId.empty()) {
				displayCourse(course);
			}
			else {
				std::cout << userCourse << " not found." << endl;
			}
			break;


		}
	}
	std::cout << "Thank you for using the course planner!" << endl;

	return 0;

}