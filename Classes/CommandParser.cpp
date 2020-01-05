#include "CommandParser.h"

using namespace std;

string CommandParser::parse(string &command){
    while (command!=""&&command[0]==' ') command.erase(0);
    while (command!=""&&command[command.size()-1]==' ') command.erase(command.size()-1);
    int i=0;
    if (command.find("login")==0){
        i+=5;
        while (i<command.size()-1&&command[i]==' '&&command[i+1]==' ') command.erase(i);
        int j=command.find_last_of(' ');
        if (j==string::npos) j=i;
        if (i==j) return "Sintaxa comenzii de login este \"login username password\"";
        return "OK login";
    }
    if (command.find("get")==0){
        i+=3;
        while (i<command.size()-1&&command[i]==' '&&command[i+1]==' ') command.erase(i);
        i++;
        if (i>=command.size()||command.substr(i).size()<3) return "Sintaxa comenzii de get este \"get nhd hh:mm\" sau \"get nha hh:mm\", unde hh:mm reprezinta o ora valida";
        if (command.substr(i,3)!="nhd"&&command.substr(i,3)!="nha") return "Sintaxa comenzii de get este \"get nhd hh:mm\" sau \"get nha hh:mm\", unde hh:mm reprezinta o ora valida";
        i+=3;
        while (i<command.size()-1&&command[i]==' '&&command[i+1]==' ') command.erase(i);
        i++;
        if (i>=command.size()||command.substr(i).size()<5) return "Sintaxa comenzii de get este \"get nhd hh:mm\" sau \"get nha hh:mm\", unde hh:mm reprezinta o ora valida";
        if (!isdigit(command[i])||!isdigit(command[i+1])||
            !isdigit(command[i+3])||!isdigit(command[i+4])||
            command[i+2]!=':')
            return "Sintaxa comenzii de get este \"get nhd hh:mm\" sau \"get nha hh:mm\", unde hh:mm reprezinta o ora valida";
        int h=(command[i]-'0')*10+command[i+1]-'0';
        int m=(command[i+3]-'0')*10+command[i+4]-'0';
        if (h>23||m>59) return "Sintaxa comenzii de get este \"get nhd hh:mm\" sau \"get nha hh:mm\", unde hh:mm reprezinta o ora valida";
        i+=5;
        if (i!=command.size()) return "Sintaxa comenzii de get este \"get nhd hh:mm\" sau \"get nha hh:mm\", unde hh:mm reprezinta o ora valida";
        return "OK get";
    }
    if (command.find("update")==0){
        i+=6;
        while (i<command.size()-1&&command[i]==' '&&command[i+1]==' ') command.erase(i);
        i++;
        if (i>=command.size()||command.substr(i).size()<10) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";
        if (!isdigit(command[i])||!isdigit(command[i+1])||command[i+2]!='.'||
            !isdigit(command[i+3])||!isdigit(command[i+4])||command[i+5]!='.'||
            !isdigit(command[i+6])||!isdigit(command[i+7])||!isdigit(command[i+8])||!isdigit(command[i+9]))
            return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";
        int day=(command[i]-'0')*10+command[i+1]-'0';
        int month=(command[i+3]-'0')*10+command[i+4]-'0';
        int year=(command[i+6]-'0')*1000+(command[i+7]-'0')*100+(command[i+8]-'0')*10+command[i+9]-'0';
        if (!isValidDate(day,month,year)) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";
        i+=10;
        while (i<command.size()-1&&command[i]==' '&&command[i+1]==' ') command.erase(i);
        i++;
        if (i>=command.size()||command.substr(i).size()<5) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";
        if (!isdigit(command[i])||!isdigit(command[i+1])||
            !isdigit(command[i+3])||!isdigit(command[i+4])||
            command[i+2]!=':')
            return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";
        int h=(command[i]-'0')*10+command[i+1]-'0';
        int m=(command[i+3]-'0')*10+command[i+4]-'0';
        if (h>23||m>59) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";
        i+=5;
        if (i!=command.size()) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data valida si hh:mm reprezinta o ora valida";        
        return "OK update";
    }
    if (command.find("logout")==0){
        i+=6;
        if (i!=command.size()) return "Comanda de logout nu are argumente";
        return "OK logout";
    }
    return "Comanda nu poate fi decat login, logout, get sau update";
}

bool CommandParser::isLeap(int year) 
{
    return (((year % 4 == 0) &&  
            (year % 100 != 0)) || 
            (year % 400 == 0)); 
} 
  
bool CommandParser::isValidDate(int d, int m, int y) 
{ 
    if (m < 1 || m > 12) 
        return false; 
    if (d < 1 || d > 31) 
        return false; 
  
    if (m == 2) 
        if (isLeap(y)) return (d <= 29); 
        else return (d <= 28); 
    if (m == 4 || m == 6 || 
        m == 9 || m == 11) 
        return (d <= 30); 
    return true; 
} 