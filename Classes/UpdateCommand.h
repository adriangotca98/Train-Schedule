#include "Command.h"
#include "../rapidxml/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class UpdateCommand: public Command{
    xml_document<> *trains, *now, *users;
    loggedUser* user;
    tm dateTime;
    void fillDateTime(string command, tm *dateTime);
    bool find(string username);
    string writeDateToString(tm tm);
    xml_node<> *allocate_node(xml_document<>* doc, string name);
    xml_attribute<> *allocate_attribute(xml_document<> *doc, string name, string value);
public:
    UpdateCommand(xml_document<> *trains, xml_document<> *now, xml_document<> *users, loggedUser *user);
    void parse(string command);
    string execute();
};