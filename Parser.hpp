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
extern map<string, int> labelMemoryID;
extern int Cur;

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
    if (ch == ':') return true;
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
        execute();
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
            int len = str.length();
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
                labelMemoryID[key.substr(0, key.length() - 1)] = pool.dataCnt();
            } else if (key != "" && ac_char(key[0])) {
                Line* L = new Line();
                LINE ++;
                L->name = key;
                while (pos < str.length()) {
                    L->argv.push_back(getKeyWord(str, pos));
                }
                if (isdata) {
                    if (dataType.count(key)) {
                        int type = dataType[key];
                        pool.addData(type, L->argv);
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
            if (L->name == "mfhi") p = new Move(s[0], "$lo");
            if (L->name == "syscall") p = new Syscall(fda, fou);
            if (L->name == "nop") p = new Instruction();
            I.push_back(p);
            //cout << L->name << " " << s[0] << " " << s[1] << " " << s[2] << endl;
         }
    }
    void execute() {
        Cur = labelLineID["main"];
        while (Cur < I.size()) {
            Instruction *p = I[Cur ++];
            cout << Cur << " : ";
            p->dataPrepare();
            p->execution();
            p->memoryAccess();
            p->writeBack();
            cout << "$a0 = " << reg[4] << " $v0 = " << reg[2] << " $sp = " << reg[29] << endl;
        }
    }
};

#endif
