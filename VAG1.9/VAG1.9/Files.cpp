#include "Files.h"

bool saveSchoolToFile(SCHOOL school) {
	ofstream file("schools.bin", ios::out | ios::app | ios::binary);
	if (file)
	{
		cout << "--------------------------------" << endl;
		cout << school.name;
		file.write((char*)&school.name, sizeof(school.name));
		file.write((char*)&school.city, sizeof(school.city));
	}
	else
	{
		return false;
	}
}

bool findSchoolFromFile(SCHOOL school) {
	ifstream file("schools.bin", ios::out | ios::binary);
	string name,city;
	if (file)
	{
		for (int i = 0; i < 2; i++)
		{
			file.read((char*)&name, sizeof(school.name));
			file.read((char*)&city, sizeof(school.city));
			cout << "----------------" << endl << name << endl << city;
		}
		
	}
	return true;
}
