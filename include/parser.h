#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <map>

#define EOL -2
using namespace std;
class Parser
{
    public:
        Parser(const char* filename): status(0), file(filename, ios::in) {}
        pair<string, string> getNextToken();
        int status;
    private:
        void mbtolower(string &mbstring) const;
        ifstream file;
};

#endif // PARSER_H
