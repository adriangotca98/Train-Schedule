#include <string>

using namespace std;

class CommandParser{
    static bool isValidDate(int,int,int);
    static bool isLeap(int);
public:
    static string parse(string&);
};