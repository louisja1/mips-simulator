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

const int Mo = 256;
const int K = 8;

//extern Memory pool;
extern map<string, int> label;
extern map<string, int> dataType;
extern map<string, int> textType;
extern map<string, int> labelLineID;
extern map<string, int> labelMemoryID;
extern int Cur;
extern int A;
extern int B;

Register reg;

bool mypredict[Mo][1 << K];
int counter[Mo][1 << K];
int S;

bool ac_char(char ch) {
    if (ch >= '0' && ch <= '9') return true;
    if (ch >= 'a' && ch <= 'z') return true;
    if (ch >= 'A' && ch <= 'Z') return true;
    if (ch == '.') return true;
    if (ch == '_') return true;
    if (ch == '$') return true;
    if (ch == ':') return true;
    if (ch == '-') return true;
    if (ch == '(' || ch == ')') return true;
    return false;
}

class Parser {
private:
    vector<Line*> Lines;
    vector<Instruction*> I;
    istream &fin;
public:
    Parser(istream &_fin) : fin(_fin) {
        Lines.clear();
        I.clear();
        input();
        classify();
        work();
        memset(mypredict, false, sizeof mypredict);
        memset(counter, 0, sizeof counter);
        S = 0;
    }
    ~Parser() {
        for (int i = 0; i < Lines.size(); i++) {
            delete Lines[i];
        }
        for (int i = 0; i < I.size(); i++) {
            delete I[i];
        }
    }
    string getKeyWord(string st, int &pos) {
        int len = st.length();
        for (; pos < len && !ac_char(st[pos]) && st[pos] != '#'; pos ++);
        string res = "";
        if (st[pos] != '#') {
            while (pos < len && ac_char(st[pos])) {
                res += st[pos];
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
            //cout << "!!!!" << str << endl;
            string key = getKeyWord(str, pos);
            //cout << "????" << key << endl;
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
                labelMemoryID[key.substr(0, key.length() - 1)] = dataCnt();
            } else if (key != "" && ac_char(key[0])) {
                Line* L = new Line();
                LINE ++;
                L->name = key;
                if (key == ".ascii" || key == ".asciiz") {
                    int le(0), ri(0);
                    for (int i = 0; i < str.length(); i++) {
                        if (str[i] == '\"') {
                            le = i; break;
                        }
                    }
                    for (int i = str.length() - 1; i >= 0; i--) {
                        if (str[i] == '\"') {
                            ri = i; break;
                        }
                    }
                    L->argv.push_back(str.substr(le + 1, ri - le - 1));
                    L->argv[0] = Tools::getRealString(L->argv[0]);
                } else {
                    while (pos < str.length()) {
                        L->argv.push_back(getKeyWord(str, pos));
                    }
                }
                if (isdata) {
                    if (dataType.count(key)) {
                        int type = dataType[key];
                        addData(type, L->argv);
                    } else {
                        throw -2;
                    }
                }
                Lines.push_back(L);
            }
        }
    }
    void classify() {
        for (int i = 0; i < Lines.size(); i++) {
            I.push_back(Lines[i]->transferToInstruction());
        }
    }
    void work() {
        Cur = labelLineID["main"];
        int regStoreTimes[34];
        int jump[3] = {0};
        for (int i = 0; i < 34; i++) regStoreTimes[i] = 0;
        Instruction *stage[5];
        for (int i = 0; i < 5; i++) stage[i] = nullptr;
        while (true) {
            //write back
            stage[4] = stage[3];
            if (stage[4] != nullptr) {
                stage[4]->writeBack();
                for (int i = 0; i < stage[4]->regStore.size(); i++) {
                    -- regStoreTimes[stage[4]->regStore[i]];
                }
                for (int i = 0; i < 3; i++) {
                    jump[i] -= stage[4]->jump[i];
                }
                stage[4] = nullptr;
            }
            //memory access
            stage[3] = stage[2];
            if (stage[3] != nullptr) {
                stage[3]->memoryAccess();
            }
            //execution
            stage[2] = stage[1];
            if (stage[2] != nullptr) {
                stage[2]->execution();
                if (stage[2]->jump[0] == 1) {
                    int id = stage[2]->line % 256;
                    ++ A;
                    bool flag = ((BranchJump*)stage[2])->flag;
                    bool predict = ((BranchJump*)stage[2])->predict;
                    if (flag != predict) {
                        ++ B;
                        if (stage[0] != nullptr) {
                            Instruction *p = stage[0];
                            for (int i = 0; i < p->regStore.size(); i++) {
                                regStoreTimes[p->regStore[i]] --;
                            }
                        }
                        stage[1] = stage[0] = nullptr;
                        if (flag) {
                            Cur = ((BranchJump*)stage[2])->id;
                        } else {
                            Cur = stage[2]->line + 1;
                        }
                        ++ counter[id][S];
                        if (counter[id][S] == 2) {
                            mypredict[id][S] = !mypredict[id][S];
                            counter[id][S] = 0;
                        }
                    } else {
                        counter[id][S] = 0;
                    }
                    S = (S >> 1) | (flag << (K - 1));
                }
            }
            //data prepare
            stage[1] = stage[0];
            if (stage[1] != nullptr) {
                stage[1]->dataPrepare();
                if (stage[1]->jump[0] == 1) {
                    int id = stage[1]->line % 256;
                    ((BranchJump*)stage[1])->predict = mypredict[id][S];
                    if (!mypredict[id][S]) Cur = stage[1]->line + 1;
                }
            }
            //Instruction fetch
            stage[0] = nullptr;
            if (Cur < I.size()) {
                while (I[Cur] == nullptr && Cur < I.size()) Cur ++;
                Instruction *p = I[Cur];
                p->line = Cur;
                bool flag = false;
                for (int i = 0; i < p->regLoad.size(); i++) {
                    if (regStoreTimes[p->regLoad[i]]) {
                        flag = true;
                        break;
                    }
                }
                for (int i = 0; i < 3; i++) {
                    if (jump[i]) {
                        flag = true;
                        break;
                    }
                }
                if (flag) {
                    p = nullptr;
                } else {
                    for (int i = 0; i < p->regStore.size(); i++) {
                        regStoreTimes[p->regStore[i]] ++;
                    }
                    for (int i = 0; i < 3; i++) {
                        jump[i] += p->jump[i];
                    }
                    Cur ++;
                }
                stage[0] = p;
            }
        }
    }
};

#endif
