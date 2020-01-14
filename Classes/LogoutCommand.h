#include "Command.h"

class LogoutCommand : public Command {
    loggedUser* user;
public:
    LogoutCommand(loggedUser* user);
    string execute();
    void parse(string command) {};
    ~LogoutCommand(){}
};