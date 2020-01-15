#include "Command.h"
#include "../rapidxml/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class DisplayDayCommand : public Command {
    xml_document<>* trains;
    loggedUser* user;
    string stationName;
    multiset<entry> entries;
public:
    DisplayDayCommand(xml_document<>* trains, loggedUser* user);
    string execute();
    void parse(string command);
    ~DisplayDayCommand(){}
};