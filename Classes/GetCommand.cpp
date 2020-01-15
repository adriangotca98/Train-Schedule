#include "GetCommand.h"
#include <vector>
#include <cstring>

GetCommand::GetCommand(xml_document<> *trains, xml_document<> *now, loggedUser *user){
    this->trains=trains;
    this->now=now;
    this->user=user;
}

void GetCommand::parse(string command){
    typeOfGet=command.substr(4,3);
    command=command.substr(8);
    memset(&dateTime,0,sizeof(dateTime));
    fillDateTime(command,&dateTime);
}

string GetCommand::execute(){
    string response="";
    string dataOra = (typeOfGet=="nhd")?"DataOraPlecare":"DataOraSosire";
    string oraAtt=(typeOfGet=="nhd")?"OraPlecare":"OraSosire";
    if (user->typeOfUser!="Station"){
        return "Only a \"Station\" type user can execute the \"get\"!\n";
    }
    vector <pair <xml_node<>*, int> > trainsWithStation;
    for (xml_node<> *train=trains->first_node()->first_node();train;train=train->next_sibling()){
        string trainId = train->first_attribute("Id")->value();
        for (auto p = make_pair(train->first_node(), 1);p.first;p.first=p.first->next_sibling(), p.second++){
            string StationName = p.first->first_attribute("Nume")->value();
            if (StationName==user->username){
                trainsWithStation.push_back({train, p.second});
            }
        }
    }
    for (auto train:trainsWithStation){
        string trainName = train.first->first_attribute("Id")->value();
        string departure = train.first->first_node()->first_attribute("Nume")->value();
        string arrival = train.first->last_node()->first_attribute("Nume")->value();
        if (typeOfGet=="nha"&&user->entityRepresented==departure) continue;
        if (typeOfGet=="nhd"&&user->entityRepresented==arrival) continue;
        bool ok=false;
        for (xml_node<> *moving=now->first_node()->first_node();moving;moving=moving->next_sibling()){
            if (string(moving->first_attribute("trainNumber")->value())==trainName){
                int delay=atoi(moving->first_attribute("delay")->value());
                int nr_station=atoi(string(moving->first_attribute("lastUpdate")->value()).substr(2).c_str());
                if (nr_station>=train.second) continue;
                xml_node<> *theory=moving->first_node();
                for (int i=1;i<nr_station;theory=theory->next_sibling(),i++);
                string dateTimeString = theory->first_attribute(dataOra.c_str())->value();
                tm dateTime;
                fillDateTime(dateTimeString,&dateTime);
                time_t numberOfSeconds = mktime(&dateTime);
                numberOfSeconds += delay*60;
                dateTime=*localtime(&numberOfSeconds);
                double timeDiff = mktime(&dateTime)-mktime(&this->dateTime);
                if (timeDiff>0&&timeDiff<=3600){
                    response+=  "Train "+trainName+" "+departure+"-"+arrival+
                                " will "+((typeOfGet=="nha")?"arrive":"depart")+" on "+asctime(&dateTime);
                    if (delay>0) response+=" later than planned with "+to_string(delay)+" minutes.";
                    if (delay<0) response+=" earlier than planned with "+to_string(delay)+" minutes.";
                    if (delay==0) response+=" right on time.";
                    response+='\n';
                    ok=true;
                }
            }
        }
        if (!ok){
            xml_node<> *stationXmlNode = train.first->first_node();
            for (int i=1;i<train.second;i++,stationXmlNode=stationXmlNode->next_sibling());
            tm tm1,tm2;
            memset(&tm1,0,sizeof(tm1));
            memset(&tm2,0,sizeof(tm2));
            string ora=stationXmlNode->first_attribute(oraAtt.c_str())->value();
            tm1.tm_hour=atoi(ora.substr(0,2).c_str());
            tm1.tm_min=atoi(ora.substr(3,2).c_str());
            tm2.tm_hour=dateTime.tm_hour;
            tm2.tm_min=dateTime.tm_min;
            int timeDiff = mktime(&tm1)-mktime(&tm2);
            if (timeDiff>0&&timeDiff<=3600){
                tm2=dateTime;
                tm1=tm2;
                tm1.tm_hour=atoi(ora.substr(0,2).c_str());
                tm1.tm_min=atoi(ora.substr(3,2).c_str());
                if (mktime(&tm1)-mktime(&tm2)<0){
                    time_t aux = mktime(&tm1+86400);
                    tm1=*localtime(&aux);
                }
                response+=  "Train "+trainName+" "+departure+"-"+arrival+
                            " will "+((typeOfGet=="nha")?"arrive":"depart")+" on "+asctime(&tm1);
            }
        }
    }
    return response;
}

void GetCommand::fillDateTime(string command, tm *dateTime){
    dateTime->tm_mday=atoi(command.substr(0,2).c_str());
    command=command.substr(3);
    dateTime->tm_mon=atoi(command.substr(0,2).c_str())-1;
    command=command.substr(3);
    dateTime->tm_year=atoi(command.substr(0,4).c_str())-1900;
    command=command.substr(5);
    dateTime->tm_hour=atoi(command.substr(0,2).c_str());
    command=command.substr(3);
    dateTime->tm_min=atoi(command.substr(0,2).c_str());
}