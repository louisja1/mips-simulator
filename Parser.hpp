#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "Register.hpp"
#include "Memory.hpp"
#include "Line.hpp"
#include "Tools.hpp"
#include "Instruction.hpp"

using namespace std;

extern Memory pool;
extern map<string, int> label;
extern map<string, int> dataType;
extern map<string, int> textType;
extern map<string, int> labelLineID;

Register reg;

int getLabelID(string st, bool isdata) {
    if (!label.count(st)) {
        label[st] = pool.distribute(isdata);
    }
    return label[st];
}

bool ac_char(char ch) {
    if (ch >= '0' && ch <= '9') return true;
    if (ch >= 'a' && ch <= 'z') return true;
    if (ch >= 'A' && ch <= 'Z') return true;
    if (ch == '.') return true;
    if (ch == '_') return true;
    if (ch == '$') return true;
    return false;
}

class Parser {
private:
    vector<Line> Lines;
    istream &fin;
    istream &fda;
    ostream &fou;
public:
    Parser(istream &_fin, istream &_fda, ostream &_fou) : fin(_fin), fda(_fda), fou(_fou){
        Lines.clear();
        input();
    }
    ~Parser() {
        for (int i = 0; i < Lines.size(); i++) {
            delete Lines[i];
        }
    }
    string getKeyWord(string st, int &pos) {
        for (; pos < len && !ac_char(s[pos]) && s[pos] != '#'; pos ++);
        string res = "";
        if (s[pos] != '#') {
            while (pos < len && ac_char(s[pos])) {
                res += s[pos];
                pos ++;
            }
        } else {
            pos = st.length();
        }
        return res;
    }
    void input() {
        fin.seekg(0, ios::beg);
        string str;
        bool isdata = false;
        int LINE = 0;
        while (getline(fin, str, '\n')) {
            int pos = 0;
            int len = str.length();
            LINE ++;
            string key = getKeyWord(str, pos);
            if (key == ".data") {
                isdata = true;
                continue;
            }
            if (key == ".text") {
                isdata = false;
                continue;
            }
            if (key[key.length() - 1] == ':') {
                labelLineID[key.substr(0, key.length() - 1)] = LINE;
                if (isdata) {
                    label_id = getLabelID(key, isdata);
                }
            } else {
                L = new line();
                L.name = key;
                while (pos < str.length()) {
                    L.argv.push_back(getKeyWord(str, pos));
                }
                L.arrange();
                if (isdata) {
                    if (dataType.count(key)) {
                        int type = dataType[key];
                        pool.addData(type, vecS);
                    } else {
                        throw -2;
                    }
                }
                Line.push_back(L);
            }
        }
        for (int i = 0; i < Line.size(); )
    }
    void work() {
        int pos = labelLineID("main");
        while (true) {
            if (Line[pos].type == instruction) {
                vector<int> G;
                for (int i = 0; i < agrv.size(); i++) {
                    if (Tools::allnumber(argv[i])) {
                        G.push_back(Tools::string2number);
                    } else
                    if (argv[i][0] == '$') {
                        G.push_back()
                    }
                }
                int type = textType[Line[pos].name];
                switch (type) : {
                    case 1 : {

                        break;
                    }
                }
            } else {
                pos ++;
                continue;
            }
        }
    }
};

#endif
