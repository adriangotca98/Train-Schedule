#include "CommandParser.h"

using namespace std;

string CommandParser::parse(string &command)
{
    while (command != "" && command[0] == ' ')
        command.erase(0,1);
    while (command != "" && command[command.size() - 1] == ' ')
        command.erase(command.size() - 1,1);
    int i = 0;
    string failLogin = "Sintaxa comenzii de login este \"login username password\"";
    string failGet = "Sintaxa comenzii de get este \"get nhd zz.ll.aaaa hh:mm\" sau \"get nha zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data calendaristica valida, iar hh:mm reprezinta o ora valida";
    string failUpdate = "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\" sau \"update zz.ll.aaaa hh:mm\", unde zz.ll.aaaa reprezinta o data calendaristica valida, iar hh:mm reprezinta o ora valida";
    if (command.find("login") == 0)
    {
        i += 5;
        while (i < command.size() - 1 && command[i] == ' ' && command[i + 1] == ' ')
            command.erase(i,1);
        int j = command.find_last_of(' ');
        if (j == string::npos)
            j = i;
        if (i == j)
            return failLogin;
        return "OK login";
    }
    if (command.find("get") == 0)
    {
        i += 3;
        while (i < command.size() - 1 && command[i] == ' ' && command[i + 1] == ' ')
            command.erase(i,1);
        i++;
        if (i >= command.size() || command.substr(i).size() < 3)
            return failGet;
        if (command.substr(i, 3) != "nhd" && command.substr(i, 3) != "nha")
            return failGet;
        i += 3;
        while (i < command.size() - 1 && command[i] == ' ' && command[i + 1] == ' ')
            command.erase(i,1);
        i++;
        if (!verifyDateAndTime(command, i))
            return failGet;
        return "OK get";
    }
    if (command.find("update") == 0)
    {
        i += 6;
        while (i < command.size() - 1 && command[i] == ' ' && command[i + 1] == ' ')
            command.erase(i,1);
        i++;
        if (!verifyDateAndTime(command,i))
            return failUpdate;
        return "OK update";
    }
    if (command.find("logout") == 0)
    {
        i += 6;
        if (i != command.size())
            return "Comanda de logout nu are argumente";
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
        if (isLeap(y))
            return (d <= 29);
        else
            return (d <= 28);
    if (m == 4 || m == 6 ||
        m == 9 || m == 11)
        return (d <= 30);
    return true;
}

bool CommandParser::verifyDateAndTime(string &command, int &i)
{
    if (i >= command.size() || command.substr(i).size() < 10)
        return false;
    if (!isdigit(command[i]) || !isdigit(command[i + 1]) || command[i + 2] != '.' ||
        !isdigit(command[i + 3]) || !isdigit(command[i + 4]) || command[i + 5] != '.' ||
        !isdigit(command[i + 6]) || !isdigit(command[i + 7]) || !isdigit(command[i + 8]) || !isdigit(command[i + 9]))
        return false;
    int day = (command[i] - '0') * 10 + command[i + 1] - '0';
    int month = (command[i + 3] - '0') * 10 + command[i + 4] - '0';
    int year = (command[i + 6] - '0') * 1000 + (command[i + 7] - '0') * 100 + (command[i + 8] - '0') * 10 + command[i + 9] - '0';
    if (!isValidDate(day, month, year))
        return false;
    i += 10;
    while (i < command.size() - 1 && command[i] == ' ' && command[i + 1] == ' ')
        command.erase(i,1);
    i++;
    if (i >= command.size() || command.substr(i).size() < 5)
        return false;
    if (!isdigit(command[i]) || !isdigit(command[i + 1]) ||
        !isdigit(command[i + 3]) || !isdigit(command[i + 4]) ||
        command[i + 2] != ':')
        return false;
    int h = (command[i] - '0') * 10 + command[i + 1] - '0';
    int m = (command[i + 3] - '0') * 10 + command[i + 4] - '0';
    if (h > 23 || m > 59)
        return false;
    i += 5;
    if (i != command.size())
        return false;
    return true;
}