#include "LogoutCommand.h"

LogoutCommand::LogoutCommand(loggedUser* user){
    this->user=user;
}

string LogoutCommand::execute(){
    if (user->username==""){
        return "No user is logged in!\n";
    }
    user->entityRepresented="";
    user->typeOfUser="";
    user->username="";
    return "User logged out!\n";
}

