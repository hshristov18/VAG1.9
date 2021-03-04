#include <iostream>
#include "Data.h"
using namespace std;
int main()
{
	STUDENT student = generateStudent();
	cout << student.firstName << endl << student.surname << endl << student.clasS << endl << student.firstRole << endl << student.email;
	TEACHER teacher = generateTeacher();
	cout << teacher.firstName << endl << teacher.surname << endl<<"TEAMS"<<endl;
	for (int i = 0; i < teacher.teams.size(); i++)
	{
		cout << teacher.teams[i]<<endl;
	}
	cout << teacher.email << endl;
}
