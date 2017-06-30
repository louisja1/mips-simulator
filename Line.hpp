#ifndef LINE_HPP
#define LINE_HPP

#include <cstdio>
#include <string>
#include <vector>
#include "Tools.hpp"

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
    virtual ~Line() {};
};


#endif
