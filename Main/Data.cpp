#include "Data.h"

//STUDENT generateStudent() {
//	cout << randomFirstName();
//}

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
	return (name[rand() %4]+to_string(rand() % 12 +1));
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