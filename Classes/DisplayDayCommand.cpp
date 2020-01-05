#include "DisplayDayCommand.h"

DisplayDayCommand::DisplayDayCommand(xml_document<> *trains, loggedUser *user)
{
    this->trains = trains;
    this->user = user;
    entries.clear();
    stationName="";
}

void DisplayDayCommand::parse(string command)
{
    stationName = command.substr(3);
}

string DisplayDayCommand::execute()
{
    for (xml_node<>* train=trains->first_node("Trenuri")->first_node("Tren");train;train=train->next_sibling()){
        entry possible;
        possible.trainNumber=train->first_attribute()->value();
        possible.departure=train->first_node("Traseu")->first_node("Statie")->first_attribute("Nume")->value();
        possible.arrival=train->first_node("Traseu")->last_node("Statie")->first_attribute("Nume")->value();
        for (xml_node<>* station=train->first_node("Traseu")->first_node("Statie");station;station=station->next_sibling()){
            string name=station->first_attribute("Nume")->value();
            string arrivalTime = station->first_attribute("OraSosire")->value();
            string departureTime = station->first_attribute("OraPlecare")->value();
            if (name==stationName){
                possible.arrivalTime=arrivalTime;
                possible.departureTime=departureTime;
                if (station==train->first_node("Traseu")->first_node("Statie"))
                    possible.isDeparture=true;
                else possible.isDeparture=false;
                if (station->next_sibling()==NULL)
                    possible.isArrival=true;
                else 
                    possible.isArrival=false;
                entries.insert(possible);
            }
        }
    }
    string response="";
    for (auto entry: entries){
        response+="Train "+entry.trainNumber+" "+entry.departure+"-"+entry.arrival+" will";
        if (entry.isDeparture==false)
            response+=" arrive at "+entry.arrivalTime;
        if (entry.isDeparture==entry.isArrival)
            response+=" and will";
        if (entry.isArrival==false)
            response+=" depart at "+entry.departureTime;
        response+="\n";
    }
    return response;
}