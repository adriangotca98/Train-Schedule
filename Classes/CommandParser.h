#include <string>

using namespace std;

class CommandParser{
    static bool isValidDate(int,int,int);
    static bool isLeap(int);
    static bool verifyDateAndTime(string&,int&);
public:
    static string parse(string&);
};