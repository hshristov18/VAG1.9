#include "SchoolGenerator.h"
#include "RandomData.h"

STUDENT generateStudent() {
	STUDENT randomStudent = { randomFirstName(),randomSurname(),randomClass(),randomRole(),randomEmail() };
	return randomStudent;
}


TEACHER generateTeacher() {
	TEACHER randomTeacher = { randomFirstName(),randomSurname(),{randomTeamName(),randomTeamName(),randomTeamName(),randomTeamName()},randomEmail() };
	return randomTeacher;
}

TEAM generateTeam() {
	TEAM randomTeam = { randomFirstName(),"Description goes here",randomDate(),randomStatus(),{generateStudent(),generateStudent(),generateStudent(),generateStudent()},	"Project name goes here",generateTeacher() };
	return randomTeam;
}

SCHOOL generateSchool() {
	SCHOOL randomSchool = { randomSchoolName(),randomCity()};
	vector<TEAM>teams;
	vector<STUDENT>students;
	vector<TEACHER>teachers;
	for (int i = 0; i < (rand()%4+3); i++)
	{
		teams.push_back(generateTeam());
		for (int j = 0; j < teams[i].students.size(); j++)
		{
			students.push_back(teams[i].students[j]);
		}
		teachers.push_back(teams[i].teacher);
	}
	randomSchool.teams = teams;
	randomSchool.students = students;
	randomSchool.teachers = teachers;
	return randomSchool;
}