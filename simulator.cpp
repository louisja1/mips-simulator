#include <cstdio>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "Parser.hpp"
#include "Memory.hpp"

typedef vector<string> vecS;
typedef unsigned int UINT;
typedef unsigned long long ULL;
typedef long long LL;

using namespace std;

Memory pool;
map<string, int> label;
map<string, int> dataType;
map<string, int> textType;
map<string, int> labelLineID;

void prep() {
    label.clear();
    dataType.clear();
    textType.clear();
    labelLineID.clear();

    dataType[".align"] = 1;
    dataType[".ascii"] = 2;
    dataType[".asciiz"] = 3;
    dataType[".byte"] = 4;
    dataType[".half"] = 5;
    dataType[".word"] = 6;
    dataType[".space"] = 7;

    textType["add"] = 1;
    textType["addu"] = 2;
    textType["addiu"] = 3;
    textType["sub"] = 4;
    textType["subu"] = 5;
    textType["mul"] = 6;
    textType["mulu"] = 7;
    textType["div"] = 8;
    textType["divu"] = 9;
    textType["xor"] = 10;
    textType["xoru"] = 11;
    textType["neg"] = 12;
    textType["negu"] = 13;
    textType["rem"] = 14;
    textType["remu"] = 15;

    textType["seq"] = 16;
    textType["sge"] = 17;
    textType["sgt"] = 18;
    textType["sle"] = 19;
    textType["slt"] = 20;
    textType["sne"] = 21;

    textType["b"] = 22;
    textType["beq"] = 23;
    textType["bne"] = 24;
    textType["bge"] = 25;
    textType["ble"] = 26;
    textType["bgt"] = 27;
    textType["blt"] = 28;
    textType["beqz"] = 29;
    textType["bnez"] = 30;
    textType["blez"] = 31;
    textType["bgez"] = 32;
    textType["bgtz"] = 33;
    textType["bltz"] = 34;
    textType["j"] = 35;
    textType["jr"] = 36;
    textType["jal"] = 37;
    textType["jalr"] = 38;

    textType["la"] = 39;
    textType["lb"] = 40;
    textType["lh"] = 41;
    textType["lw"] = 42;

    textType["sb"] = 43;
    textType["sh"] = 44;
    textType["sw"] = 45;

    textType["move"] = 46;
    textType["mfhi"] = 47;
    textType["mflo"] = 48;

    textType["nop"] = 49;
    textType["syscall"] = 50;
}

int main(int argc, char *argv[]) {
    string mipsInFile, dataInFile;
    if (argc == 1) {
        cout << "Please input the mips file: ";
        cin >> mipsInFile;
        argc ++;
    } else {
        mipsInFile = argv[1];
    }
    if (argc == 2) {
        cout << "Please output the data file: ";
        cin >> dataInFile;
        argc ++;
    } else {
        dataInFile = argv[2];
    }

    ifstream mipsIn(mipsInFile);
    ifstream dataIn(dataInFile);
    ofstream dataAns("output.txt");

    prep();
    Parser ps(mipsIn, dataIn, dataAns);
    ps.work();

    mipsIn.close();
    dataIn.close();

    return 0;
}
