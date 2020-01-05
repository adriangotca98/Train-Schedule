#include "Command.h"
#include "../rapidxml/rapidxml.hpp"
using namespace std;
using namespace rapidxml;

class LoginCommand: public Command
{
    xml_document<> *users, *trains, *now;
    loggedUser *user;
    string username;
    string pass;
public:
    LoginCommand(xml_document<>* users, xml_document<>* trains, xml_document<>* now, loggedUser *user);
    void parse(string command);
    string execute();
};