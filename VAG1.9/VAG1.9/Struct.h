#pragma once
#include <string>
#include <vector>
using namespace std;
struct STUDENT
{
	string firstName;
	string surname;
	string clasS;
	string firstRole;
	//string secondRole;
	string email;
};
struct TEACHER {
	string firstName;
	string surname;
	vector<string> teams;
	string email;
};
struct TEAM
{
	string name;
	string description;
	string date; // dd/mm/yyyy
	string status;
	vector<STUDENT> students;
	string project;
	TEACHER teacher;
};
struct SCHOOL
{
	string name;
	string city;
	vector<TEACHER> teachers;
	vector<STUDENT> students;
	vector<TEAM> teams;
};