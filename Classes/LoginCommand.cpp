#include "LoginCommand.h"


LoginCommand::LoginCommand(xml_document<>* users, xml_document<>* trains, xml_document<>* now, loggedUser *user){
    this->now=now;
    this->users = users;
    this->trains=trains;
    this->user=user;
}

void LoginCommand::parse(string command){
	int start = command.find_first_of(' ')+1;
	int stop = command.find_last_of(' ')-1;
	username = command.substr(start,stop-start+1);
	pass = command.substr(command.find_last_of(' ')+1);
}

string LoginCommand::execute(){
	string response="";
	if (user->username != "") {
		return "An user is already loggedd in!\n";
	}
	for (xml_node<>* credential = users->first_node()->first_node(); credential; credential = credential->next_sibling()) {
		string username = credential->first_attribute("username")->value();
		if (this->username == username) {
			string pass = credential->first_attribute("password")->value();
			if (this->pass == pass) {
                user->username = username;
				user->typeOfUser = credential->first_attribute("Type")->value();
				user->entityRepresented = credential->first_attribute("represents")->value();
				return "Login successful!";
			}
			else {
				return "Wrong password!\n";
			}
		}
	}
	return "User " + username + " not found!\n";
}