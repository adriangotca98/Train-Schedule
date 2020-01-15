#include "Command.h"
#include "../rapidxml/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class GetCommand : public Command
{
    xml_document<> *trains, *now;
    loggedUser *user;
    tm dateTime;
    string typeOfGet;
    void fillDateTime(string command, tm *dateTime);
public:
    GetCommand(xml_document<> *trains, xml_document<> *now, loggedUser *user);
    string execute();
    void parse(string command);
};