#include "RandomData.h"



string randomTeamName() {
	string name[16] = { "Phoenix Strikers", "Team 404", "Technical knockout", "DJ Dreamers",
					"VAG4Life", "Pasta source", "varr", "Open pasta",
					"GUI haters", "SQL army", "Never gonna give you up", "K racers",
					"POG", "italian restaurant", "Pasta chefs", "Trohpy thieves" };
	return name[rand() % 16];
}
string randomFirstName() {
	string name[28] = { "Ivan", "Svetlio", "Kondio","Svetlana","Petyr","Ivana","Stanimira",
						"Stilqn","Alfredo","Dimitar","Stoqn","Iliyan","Emili","Martin",
						"Mario","Stanislav","Kaloqn","Alexandra","Alexandar","Gabriela", "Stoicho",
						"Kristian","Ema","Hristo","Milka","Qvor","Georgi","Simona" };
	return name[(rand() % 28)];
}
string randomSurname() {
	string name[28] = { "Terziev", "Ignatov", "Spasova","Trendafilova","Ivanova","Strahieva","Krumov",
						"Stilqnov","Alfredov","Dimitrov","Stoqnov","Iliyanev","Dimova","Martinov",
						"Andrikov","Stanislavov","Kaloqnov","Petkov","Zlatanov","Spasova", "Stoichov",
						"Kristianov","Strahilov","Hristov","Katyrova","Qvorov","Georgiev","Todorov" };
	return name[(rand() % 28)];
}
string randomClass() {
	string name[4] = { "A","B","V","G", };
	return (to_string(rand() % 12 + 1) + name[rand() % 4]);
}
string randomRole() {
	string roles[4] = { "Scrum Trainer","Q&A engineer","Developer Backend","Developer Frontend" };
	return roles[rand() % 4];
}

string randomEmail() {
	string emails[19] = { "skitnika32","batkata42","kampus132","ninjabg65","1.9kosmos","babatarator",
						"kamparov67","hookahmaster80","bylgariqnadvsichko","naegipetfaroona","fakerfan12","karboratob",
						"abv.bg","gmail.com","yahoo.com","hotmail.com","codingburgas.bg","uban.com","vmail.net" };
	return emails[(rand() % 12)] + "@" + emails[(rand() % 7 + 12)];
}

string randomDate() {
	string day, year, month;
	year = to_string(rand() % 30 + 2000);
	month = to_string(rand() % 12 + 1);
	
	if (month == "1" || month == "3" || month == "5" || month == "8" || month == "7" || month == "10" || month == "12")
	{
		return to_string(rand() % 31 + 1) + "/" + month + "/" + year;
	}
	else if (stoi(year) % 4 == 0 && month == "2") {
		return to_string(rand() % 29 + 1) + "/" + month + "/" + year;
	}
	else if (month=="2") {
		return to_string(rand() % 28 + 1) + "/" + month + "/" + year;
	}
	return to_string(rand() % 30 + 1) + "/" + month + "/" + year;
	
}

string randomStatus() {
	string st[3] = {"In use", "Not active", "Archived"};
	return st[rand() % 3];
}

string randomSchoolName() {
	string name[12] = { "Preslavski","Mehano","Elektro","Toha",
					"Vasil Aprilov","Tyrgovska","Muzikalno","Himiqta",
					"Kiril i Metodi", "Morskoto","PGKPI","Matematicheska" };
	return name[rand() % 12] + to_string(rand() % 98 + 1);
}

string randomCity() {
	string citys[20] = { "Burgas","Yambol","Varna","Sofiq", "Silistra",
					"Gabrovo", "Pernik", "Kazanlyk","Koprivchica","Bansko",
					"Plovdiv", "Shumen", "Lovech", "Vidin", "Smolqn",
					"Ruse","Pleven", "Dobrich","Stara Zagora", "Karlovo" };
	return citys[rand() % 20];
}