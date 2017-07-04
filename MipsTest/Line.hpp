#ifndef LINE_HPP
#define LINE_HPP

#include <cstdio>
#include <string>
#include <vector>
#include "Tools.hpp"

typedef vector<string> vecS;

using namespace std;

class Line {
    friend class Parser;
protected:
    vecS argv;
    string name;
public:
    Line() : name("") {
        argv.clear();
    }
    ~Line() {};
};


#endif
