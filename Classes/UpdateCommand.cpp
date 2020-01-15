#include "UpdateCommand.h"
#include <cstring>

UpdateCommand::UpdateCommand(xml_document<> *trains, xml_document<> *now, xml_document<> *users, loggedUser *user)
{
    this->trains = trains;
    this->now = now;
    this->user = user;
    this->users = users;
}

void UpdateCommand::parse(string command)
{
    command = command.substr(7);
    fillDateTime(command, &dateTime);
}

void UpdateCommand::fillDateTime(string command, tm *dateTime)
{
    memset(dateTime, 0, sizeof(tm));
    dateTime->tm_mday = atoi(command.substr(0, 2).c_str());
    command = command.substr(3);
    dateTime->tm_mon = atoi(command.substr(0, 2).c_str()) - 1;
    command = command.substr(3);
    dateTime->tm_year = atoi(command.substr(0, 4).c_str()) - 1900;
    command = command.substr(5);
    dateTime->tm_hour = atoi(command.substr(0, 2).c_str());
    command = command.substr(3);
    dateTime->tm_min = atoi(command.substr(0, 2).c_str());
}

string UpdateCommand::execute()
{
    string response = "";
    if (user->typeOfUser != "Train")
    {
        return "Only a \"Train\" type user can execute the \"get\"!\n";
    }
    if (user->entityRepresented == user->username)
    {
        int i = 1;
        while (find(user->username + "_" + to_string(i)))
            i++;
        string newUsername = user->username + "_" + to_string(i);
        xml_node<> *trainInNow = allocate_node(now, "train");
        xml_attribute<> *trainNumber = allocate_attribute(now,"trainNumber",user->username);
        xml_attribute<> *lastUpdate = allocate_attribute(now,"lastUpdate", "d_1");
        xml_attribute<> *id = allocate_attribute(now, "Id", newUsername);
        xml_node<> *train = trains->first_node()->first_node();
        for (; string(train->first_attribute("Id")->value()) != user->username; train = train->next_sibling());
        xml_node<> *stationXmlNode=train->first_node();
        string ora = stationXmlNode->first_attribute("OraPlecare")->value();
        tm tm1,tm2;
        memset(&tm1,0,sizeof(tm));
        memset(&tm2,0,sizeof(tm));
        tm1.tm_hour = atoi(ora.substr(0, 2).c_str());
        tm1.tm_min = atoi(ora.substr(3, 2).c_str());
        tm2.tm_hour = dateTime.tm_hour;
        tm2.tm_min = dateTime.tm_min;
        int timeDiff = mktime(&tm2) - mktime(&tm1);
        int scad=0;
        if (timeDiff<0){
            timeDiff+=86400;
            scad=86400;
        }
        xml_attribute<> *delay=allocate_attribute(now, "delay", to_string(timeDiff/60));
        trainInNow->append_attribute(trainNumber);
        trainInNow->append_attribute(lastUpdate);
        trainInNow->append_attribute(id);
        trainInNow->append_attribute(delay);
        tm1.tm_mday=dateTime.tm_mday;
        tm1.tm_mon=dateTime.tm_mon;
        tm1.tm_year=dateTime.tm_year;
        time_t convert = mktime(&tm1)-scad;
        tm1=*localtime(&convert);
        xml_node<> *station=allocate_node(now, "Statie");
        string dateAsString = writeDateToString(tm1);
        xml_attribute<> *dataOraSosire=allocate_attribute(now,"DataOraSosire", dateAsString);
        xml_attribute<> *dataOraPlecare=allocate_attribute(now,"DataOraPlecare",dateAsString);
        xml_attribute<> *nume=allocate_attribute(now,"Nume",stationXmlNode->first_attribute("Nume")->value());
        station->append_attribute(dataOraPlecare);
        station->append_attribute(dataOraSosire);
        station->append_attribute(nume);
        trainInNow->append_node(station);
        for (xml_node<> *station=train->first_node()->next_sibling();station;stationXmlNode=station,station=station->next_sibling()){
            xml_node<> *stationInNow = allocate_node(now,"Statie");
            xml_attribute<> *nume =allocate_attribute(now,"Nume",station->first_attribute("Nume")->value());
            xml_attribute<> *dataOraSosire = allocate_attribute(now,"DataOraSosire","");
            xml_attribute<> *dataOraPlecare = allocate_attribute(now,"DataOraPlecare","");
            string hour=station->first_attribute("OraSosire")->value();
            tm2=tm1;
            tm2.tm_hour=atoi(hour.substr(0,2).c_str());
            tm2.tm_min=atoi(hour.substr(3,2).c_str());
            int diffTime=difftime(mktime(&tm2),mktime(&tm1));
            if (diffTime<0) convert = mktime(&tm2)+86400, tm2=*localtime(&convert);
            tm1=tm2;
            string dateAsString = writeDateToString(tm1);
            dataOraSosire->value(now->allocate_string(dateAsString.c_str()));
            hour=station->first_attribute("OraPlecare")->value();
            tm2=tm1;
            tm2.tm_hour=atoi(hour.substr(0,2).c_str());
            tm2.tm_min=atoi(hour.substr(3,2).c_str());
            diffTime=difftime(mktime(&tm2),mktime(&tm1));
            if (diffTime<0) convert = mktime(&tm2)+86400, tm2=*localtime(&convert);
            tm1=tm2;
            dateAsString = writeDateToString(tm1);
            dataOraPlecare->value(now->allocate_string(dateAsString.c_str()));
            stationInNow->append_attribute(nume);
            stationInNow->append_attribute(dataOraSosire);
            stationInNow->append_attribute(dataOraPlecare);
            trainInNow->append_node(stationInNow);
        }
        now->first_node()->append_node(trainInNow);
        xml_node<> *userInUsers=users->first_node()->first_node();
        while (string(userInUsers->first_attribute("username")->value())!=user->username) userInUsers=userInUsers->next_sibling();
        string password=userInUsers->first_attribute("password")->value();
        user->username=newUsername;
        xml_node<> *newUser = allocate_node(users,"user");
        xml_attribute<> *username=allocate_attribute(users,"username",newUsername);
        xml_attribute<> *type=allocate_attribute(users,"Type",user->typeOfUser);
        xml_attribute<> *represents=allocate_attribute(users,"represents",user->entityRepresented);
        xml_attribute<> *pass=allocate_attribute(users,"password",password);
        newUser->append_attribute(username);
        newUser->append_attribute(type);
        newUser->append_attribute(represents);
        newUser->append_attribute(pass);
        users->first_node()->append_node(newUser);
        return "First update successful! New user for updates for this train: "+newUsername+"\n";
    }
    else
    {
        for (xml_node<> *moving = now->first_node()->first_node(); moving; moving = moving->next_sibling())
        {
            string id = moving->first_attribute("Id")->value();
            string trainNumber = moving->first_attribute("trainNumber")->value();
            string lastUpdate = moving->first_attribute("lastUpdate")->value();
            if (id == user->username)
            {
                int stationNumber = atoi(lastUpdate.substr(2).c_str());
                if (lastUpdate[0] == 'd')
                    stationNumber++;
                xml_node<> *station = moving->first_node();
                for (int i = 1; i < stationNumber; i++, station = station->next_sibling());
                string oraAtt = (lastUpdate[0] == 'd') ? "DataOraSosire" : "DataOraPlecare";
                tm theory;
                fillDateTime(station->first_attribute(oraAtt.c_str())->value(), &theory);
                int delay = difftime(mktime(&dateTime), mktime(&theory)) / 60;
                lastUpdate[0] = (lastUpdate[0] == 'd') ? 'a' : 'd';
                lastUpdate=lastUpdate.substr(0,2);
                lastUpdate+=to_string(stationNumber);
                moving->first_attribute("lastUpdate")->value(now->allocate_string(lastUpdate.c_str()));
                moving->first_attribute("delay")->value(now->allocate_string(to_string(delay).c_str()));
            }
        }
        return "Update successful!";
    }
}

bool UpdateCommand::find(string username)
{
    for (xml_node<> *user = users->first_node()->first_node(); user; user = user->next_sibling())
    {
        string userName = user->first_attribute("username")->value();
        if (userName == username)
            return true;
    }
    return false;
}

string UpdateCommand::writeDateToString(tm tm){
    string date="";
    if (tm.tm_mday<10) date+="0";
    date+=to_string(tm.tm_mday)+".";
    if (tm.tm_mon<9) date+="0";
    date+=to_string(tm.tm_mon+1)+".";
    date+=to_string(tm.tm_year+1900)+" ";
    if (tm.tm_hour<10) date+="0";
    date+=to_string(tm.tm_hour)+":";
    if (tm.tm_min<10) date+="0";
    date+=to_string(tm.tm_min);
    return date;
}

xml_node<>* UpdateCommand::allocate_node(xml_document<> *doc, string name){
    return doc->allocate_node(node_element, doc->allocate_string(name.c_str()));
}

xml_attribute<>* UpdateCommand::allocate_attribute(xml_document<> *doc, string name, string value){
    return doc->allocate_attribute(doc->allocate_string(name.c_str()),doc->allocate_string(value.c_str()));
}