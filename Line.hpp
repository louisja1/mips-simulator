#ifndef LINE_HPP
#define LINE_HPP

#include <cstdio>
#include <string>
#include <vector>
#include "Tools.hpp"
#include "Instruction.hpp"

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
    Instruction* transferToInstruction() {
        for (int i = argv.size(); i < 3; i++) {
            argv.push_back("");
        }
        if (name == "add") return new Add(argv[0], argv[1], argv[2], false);
        if (name == "addu") return new Add(argv[0], argv[1], argv[2], true);
        if (name == "addiu") return new Add(argv[0], argv[1], argv[2], true);
        if (name == "sub") return new Sub(argv[0], argv[1], argv[2], false);
        if (name == "subu") return new Sub(argv[0], argv[1], argv[2], true);
        if (name == "mul") return new  Mul(argv[0], argv[1], argv[2], false);
        if (name == "mulu") return new Mul(argv[0], argv[1], argv[2], true);
        if (name == "div") return new Div(argv[0], argv[1], argv[2], false);
        if (name == "divu") return new Div(argv[0], argv[1], argv[2], true);
        if (name == "xor") return new Xor(argv[0], argv[1], argv[2], false);
        if (name == "xoru") return new Xor(argv[0], argv[1], argv[2], true);
        if (name == "neg") return new Neg(argv[0], argv[1], false);
        if (name == "negu") return new Neg(argv[0], argv[1], true);
        if (name == "rem") return new Rem(argv[0], argv[1], argv[2], false);
        if (name == "remu") return new Rem(argv[0], argv[1], argv[2], true);
        if (name == "li") return new Move(argv[0], argv[1]);
        if (name == "seq") return new Seq(argv[0], argv[1], argv[2]);
        if (name == "sge") return new Sge(argv[0], argv[1], argv[2]);
        if (name == "sgt") return new Sgt(argv[0], argv[1], argv[2]);
        if (name == "sle") return new Sle(argv[0], argv[1], argv[2]);
        if (name == "slt") return new Slt(argv[0], argv[1], argv[2]);
        if (name == "sne") return new Sne(argv[0], argv[1], argv[2]);
        if (name == "b") return new Goto(argv[0]);
        if (name == "beq") return new Beq(argv[0], argv[1], argv[2]);
        if (name == "bne") return new Bne(argv[0], argv[1], argv[2]);
        if (name == "bge") return new Bge(argv[0], argv[1], argv[2]);
        if (name == "ble") return new Ble(argv[0], argv[1], argv[2]);
        if (name == "bgt") return new Bgt(argv[0], argv[1], argv[2]);
        if (name == "blt") return new Blt(argv[0], argv[1], argv[2]);
        if (name == "beqz") return new Beq(argv[0], "", argv[1]);
        if (name == "bnez") return new Bne(argv[0], "", argv[1]);
        if (name == "bgez") return new Bge(argv[0], "", argv[1]);
        if (name == "blez") return new Ble(argv[0], "", argv[1]);
        if (name == "bgtz") return new Bgt(argv[0], "", argv[1]);
        if (name == "bltz") return new Blt(argv[0], "", argv[1]);
        if (name == "j") return new Goto(argv[0]);
        if (name == "jr") return new Goto(argv[0]);
        if (name == "jal") return new Jal(argv[0]);
        if (name == "jalr") return new Jal(argv[0]);
        if (name == "la") return new La(argv[0], argv[1]);
        if (name == "lb") return new Lb(argv[0], argv[1]);
        if (name == "lh") return new Lh(argv[0], argv[1]);
        if (name == "lw") return new Lw(argv[0], argv[1]);
        if (name == "sb") return new Sb(argv[0], argv[1]);
        if (name == "sh") return new Sh(argv[0], argv[1]);
        if (name == "sw") return new Sw(argv[0], argv[1]);
        if (name == "move") return new Move(argv[0], argv[1]);
        if (name == "mfhi") return new Move(argv[0], "$hi");
        if (name == "mflo") return new Move(argv[0], "$lo");
        if (name == "syscall") return new Syscall(cin, cout);
        if (name == "nop") return new Instruction();
        return nullptr;
    }
};


#endif
