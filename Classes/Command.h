#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <set>
#include <iostream>

using namespace std;

struct loggedUser{
	string username;
	string typeOfUser;
	string entityRepresented;
};

struct entry{
    string trainNumber;
    string departure;
    string arrival;
    string arrivalTime;
    string departureTime;
    bool isDeparture;
    bool isArrival;
    friend bool operator<(const entry &e1, const entry& e2){
        return e1.arrivalTime<e2.arrivalTime||(e1.arrivalTime==e2.arrivalTime&&e1.departureTime<e2.departureTime);
    }
};

class Command
{
public:
    Command() {}
    ~Command() {}
    virtual string execute() = 0;
    virtual void parse(string s) = 0;
};

#endif