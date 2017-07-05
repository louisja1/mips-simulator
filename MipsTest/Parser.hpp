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

//extern Memory pool;
extern map<string, int> label;
extern map<string, int> dataType;
extern map<string, int> textType;
extern map<string, int> labelLineID;
extern map<string, int> labelMemoryID;
extern int Cur;

Register reg;

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
    istream &fda;
    ostream &fou;
public:
    Parser(istream &_fin, istream &_fda, ostream &_fou) : fin(_fin), fda(_fda), fou(_fou){
        Lines.clear();
        I.clear();
        input();
        classify();
        work();
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
            Line* L = Lines[i];
            string s[3];
            for (int i = 0; i < 3; i++) {
                s[i] = "";
            }
            for (int i = 0; i < L->argv.size(); i++) {
                s[i] = L->argv[i];
            }
            Instruction *p = nullptr;
            if (L->name == "add") p = new Add(s[0], s[1], s[2], false);
            if (L->name == "addu") p = new Add(s[0], s[1], s[2], true);
            if (L->name == "addiu") p = new Add(s[0], s[1], s[2], true);
            if (L->name == "sub") p = new Sub(s[0], s[1], s[2], false);
            if (L->name == "subu") p = new Sub(s[0], s[1], s[2], true);
            if (L->name == "mul") p = new  Mul(s[0], s[1], s[2], false);
            if (L->name == "mulu") p = new Mul(s[0], s[1], s[2], true);
            if (L->name == "div") p = new Div(s[0], s[1], s[2], false);
            if (L->name == "divu") p = new Div(s[0], s[1], s[2], true);
            if (L->name == "xor") p = new Xor(s[0], s[1], s[2], false);
            if (L->name == "xoru") p = new Xor(s[0], s[1], s[2], true);
            if (L->name == "neg") p = new Neg(s[0], s[1], false);
            if (L->name == "negu") p = new Neg(s[0], s[1], true);
            if (L->name == "rem") p = new Rem(s[0], s[1], s[2], false);
            if (L->name == "remu") p = new Rem(s[0], s[1], s[2], true);
            if (L->name == "li") p = new Move(s[0], s[1]);
            if (L->name == "seq") p = new Seq(s[0], s[1], s[2]);
            if (L->name == "sge") p = new Sge(s[0], s[1], s[2]);
            if (L->name == "sgt") p = new Sgt(s[0], s[1], s[2]);
            if (L->name == "sle") p = new Sle(s[0], s[1], s[2]);
            if (L->name == "slt") p = new Slt(s[0], s[1], s[2]);
            if (L->name == "sne") p = new Sne(s[0], s[1], s[2]);
            if (L->name == "b") p = new Goto(s[0]);
            if (L->name == "beq") p = new Beq(s[0], s[1], s[2]);
            if (L->name == "bne") p = new Bne(s[0], s[1], s[2]);
            if (L->name == "bge") p = new Bge(s[0], s[1], s[2]);
            if (L->name == "ble") p = new Ble(s[0], s[1], s[2]);
            if (L->name == "bgt") p = new Bgt(s[0], s[1], s[2]);
            if (L->name == "blt") p = new Blt(s[0], s[1], s[2]);
            if (L->name == "beqz") p = new Beq(s[0], "", s[1]);
            if (L->name == "bnez") p = new Bne(s[0], "", s[1]);
            if (L->name == "bgez") p = new Bge(s[0], "", s[1]);
            if (L->name == "blez") p = new Ble(s[0], "", s[1]);
            if (L->name == "bgtz") p = new Bgt(s[0], "", s[1]);
            if (L->name == "bltz") p = new Blt(s[0], "", s[1]);
            if (L->name == "j") p = new Goto(s[0]);
            if (L->name == "jr") p = new Goto(s[0]);
            if (L->name == "jal") p = new Jal(s[0]);
            if (L->name == "jalr") p = new Jal(s[0]);
            if (L->name == "la") p = new La(s[0], s[1]);
            if (L->name == "lb") p = new Lb(s[0], s[1]);
            if (L->name == "lh") p = new Lh(s[0], s[1]);
            if (L->name == "lw") p = new Lw(s[0], s[1]);
            if (L->name == "sb") p = new Sb(s[0], s[1]);
            if (L->name == "sh") p = new Sh(s[0], s[1]);
            if (L->name == "sw") p = new Sw(s[0], s[1]);
            if (L->name == "move") p = new Move(s[0], s[1]);
            if (L->name == "mfhi") p = new Move(s[0], "$hi");
            if (L->name == "mflo") p = new Move(s[0], "$lo");
            if (L->name == "syscall") p = new Syscall(fda, fou);
            if (L->name == "nop") p = new Instruction();
            I.push_back(p);
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
            if (stage[3] != nullptr) {
                stage[3]->memoryAccess();
            }
            stage[4] = stage[3];
            stage[3] = nullptr;
            //execution
            if (stage[2] != nullptr) {
                stage[2]->execution();
            }
            stage[3] = stage[2];
            stage[2] = nullptr;
            //data prepare
            if (stage[1] != nullptr) {
                stage[1]->dataPrepare();
            }
            stage[2] = stage[1];
            //Instruction fetch
            stage[1] = stage[0];
            stage[0] = nullptr;
            if (Cur < I.size()) {
                while (I[Cur] == nullptr && Cur < I.size()) Cur ++;
                Instruction *p = I[Cur];
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
