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
	TEAM randomTeacher = { randomFirstName(),"Description goes here",,randomEmail() };
	return randomTeacher;
}