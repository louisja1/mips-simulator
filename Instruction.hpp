#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdio>
#include <string>

using namespace std;

class Instruction {
public:
    Instruction() {}
    virtual ~Instruction() {}
    virtual void dataPrepare() {}
    virtual void execution() {}
    virtual void memoryAccess() {}
    virtual void writeBack() {}
}

//caculation && const operation && comparation
class Caculation : public Instruction {
public:
    int rdest, rsrc1, rsrc2;
    int imm1, imm2, result;
    Caculation(const string &_rdest, const string &_rsrc1, const string &_rsrc2) : Instruction() {
        rdest = Tools::getRegister(_rdest);
        if (Tools::allnumber(_rsrc1)) {
            imm1 = Tools::string2number(_rsrc1);
            rsrc1 = -1;
        } else {
            rsrc1 = Tools::getRegister(_rsrc1);
        }
        if (Tools::allnumber(_rsrc2)) {
            imm2 = Tools::string2number(_rsrc2);
            rsrc2 = -1;
        } else {
            rsrc2 = Tools::getRegister(_rsrc2);
        }
    }
    virtual ~Caculation() {}
    virtual void dataPrepare() {
        if (rsrc1 != -1) imm1 = reg[rsrc1];
        if (rsrc2 != -1) imm2 = reg[rsrc2];
    }
    virtual void writeBack() {
        reg[rdest] = result;
    }
};

class Add : public Caculation {
    bool unsign;
    Add(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {}
    virtual void execution() { res = imm1 + imm2; }
};

class Sub : public Caculation {
    bool unsign;
    Sub(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {}
    virtual void execution() { res = imm1 - imm2; }
};

class Mul : public Caculation {
    bool unsign;
    bool isempty;
    LL ans;
    Sub(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {
            isempty = (_rsrc3 == "");
        }
    virtual void dataPrepare() {
        if (isempty) {
            imm2 = imm1;
            if (rsrc1 != -1) imm2 = reg[rsrc1];
            imm1 = reg[rdset];
        } else {
            if (rsrc1 != -1) imm1 = reg[rsrc1];
            if (rsrc2 != -1) imm2 = reg[rsrc2];
        }
    }
    virtual void execution() {
        if (unsign) {
            ans = 1ULL * UINT(imm1) * UINT(imm2);
        } else {
            ans = 1LL * imm1 * imm2;
        }
    }
    virtual void writeBack() {
        if (isempty) {
            reg.lo = ans % (1LL << 32);
            reg.hi = ans >> 32;
        } else {
            reg[rdset] = ans;
        }
    }
};

class Div : public Caculation {
    bool unsign;
    bool isempty;
    int quo, rem;
    Sub(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {
            isempty = (_rsrc3 == "");
        }
    virtual void dataPrepare() {
        if (isempty) {
            imm2 = imm1;
            if (rsrc1 != -1) imm2 = reg[rsrc1];
            imm1 = reg[rdset];
        } else {
            if (rsrc1 != -1) imm1 = reg[rsrc1];
            if (rsrc2 != -1) imm2 = reg[rsrc2];
        }
    }
    virtual void execution() {
        if (unsign) {
            quo = UINT(imm1) / UINT(imm2);
            rem = UINT(imm1) % UINT(imm2);
        } else {
            quo = imm1 / imm2;
            rem = imm1 % imm2;
        }
    }
    virtual void writeBack() {
        if (isempty) {
            reg.lo = quo;
            reg.hi = rem;
        } else {
            reg[rdset] = quo;
        }
    }
};

//branch && jump
class BranchJump : public Instruction {
public:
    int rsrc1, rsrc2;
    int imm1, imm2;
    bool flag;
    int id;

    BranchJump(const string &_rsrc1, const string &_rsrc2, const string &_label) : Instruction() {
        if (Tools::allnumber(_rsrc1)) {
            imm1 = Tools::string2number(_rsrc1);
            rsrc1 = -1;
        } else {
            rsrc1 = Tools::Tools::getRegister(_rsrc1);
        }
        if (Tools::allnumber(_rsrc2)) {
            imm2 = Tools::string2number(_rsrc2);
            rsrc2 = -1;
        } else {
            rsrc2 = Tools::getRegister(_rsrc2);
        }
        flag = false;
        id = labelLineID[_label];
    }
    virtual ~BranchJump() {}
    virtual void dataPrepare() {
        if (rsrc1 != -1) imm1 = reg[rsrc1];
        if (rsrc2 != -1) imm2 = reg[rsrc2];
    }
    virtual void writeBack() {
        if (flag) {
            Cur = id;
        }
    }
};

//b j jr
class Goto : public Instruction {
public:
    int rdest, id;
    Goto(const string &_rdest) : Instruction() {
        if (labelLineID.count(_rdest) == 0) {
            rdest = Tools::getRegister(_rdest);
        } else {
            rdest = -1;
            id = labelLineID(_rdest);
        }
    }
    virtual ~Goto() {}
    virtual void dataPrepare() {
        if (rdest != -1) id = reg[rdest];
    }
    virtual void writeBack() {
        Cur = id;
    }
};

//jal jalr
class Jal : public Instruction{
public:
    int rsrc, id;
    Jal(const string &_rsrc) {
        if (labelLineID.count(_rsrc) == 0) {
            rsrc = Tools::getRegister(_rsrc);
        } else {
            rsrc = -1;
            id = labelLineID(_rsrc);
        }
    }
    virtual ~Jal() {}
    virtual void dataPrepare() {
        if (rsrc != -1) id = reg[rsrc];
    }
    virtual void writeBack() {
        reg[31] = Cur;
        Cur = id;
    }
};

//load
class Load : public Instruction {
public:
    int rdset;
    string address;
    int id, storage;
    Load(const string &_rdset, const string &_address)
        : Instruction(), rdset(Tools::getRegister(rdset)), address(_address), id(-1), storage(0) {}
    virtual ~Load() {}
    virtual void execution() {
        if (labelLineID.count(address) > 0) {
            id = labelLineID[address];
        } else {
            int le, ri;
            for (int i = 0; i < address.length(); i++) {
                if (address[i] == '(') le = i;
                if (address[i] == ')') ri = j;
            }
            int origin = reg[Tools::getRegister(address.substr(i + 1, j - i - 1))];
            int offset = Tools::string2number(address.substr(0, i));
            id = origin + offset;
        }
    }
    virtual void memoryAccess() {}
    virtual void writeBack() {
        reg[rdset] = storage;
    }
};

//store
class Store : public Instruction {
public:
    int rdset;
    string address;
    int id, value;
    Store(const string &_rdset, const string &_address)
        : Instruction(), rdset(Tools::getRegister(rdset)), address(_address), id(-1), value(0) {}
    virtual ~Store() {}
    virtual void dataPrepare() {
        value = reg[rdset];
    }
    virtual void execution() {
        if (labelLineID.count(address) > 0) {
            id = labelLineID[address];
        } else {
            int le, ri;
            for (int i = 0; i < address.length(); i++) {
                if (address[i] == '(') le = i;
                if (address[i] == ')') ri = j;
            }
            int origin = reg[Tools::getRegister(address.substr(i + 1, j - i - 1))];
            int offset = Tools::string2number(address.substr(0, i));
            id = origin + offset;
        }
    }
    virtual void memoryAccess() {}
};

//move
class Move : public Instruction {
public:
    int rdset, rsrc, imm;
    Move(const string &_rdset, const string &_rsrc) : Instruction() {
        rdset = Tools::getRegister(_rdset);
        if (Tools::allnumber(_rsrc)) {
            imm = Tools::string2number(_rsrc);
            rsrc = -1;
        } else {
            rsrc = Tools::getRegister(_rsrc);
        }
    }
    virtual ~Move() {}
    virtual void dataPrepare() {
        if (rsrc != -1) imm = reg[rsrc];
    }
    virtual void writeBack() {
        reg[rdset] = imm;
    }
};

//syscall
class Syscall : public Instruction {
public:
    istream &fda;
    ostream &fou;
    int a0, a1;
    int ope, pos;
    int Int;
    string Str;
    Syscall(istream &_fda, ostream &fou) : fda(_fda), fou(_fou) {}
    virtual void dataPrepare() {
        ope = reg[2];
        switch (ope) {
            case 1 : { a0 = reg[4]; break; }
            case 4 : { a0 = reg[4]; break; }
            case 5 : { break; }
            case 8 : { a0 = reg[4]; a1 = reg[5]; break; }
            case 9 : { a0 = reg[4]; break; }
            case 10 : { break; }
            case 17 : { a0 = reg[4]; break; }
        }
    }
    virtual void execution() {
        switch (ope) {
            case 1 : { fou << a0; break; }
            case 5 : { fin >> Int; break; }
            case 8 : { fin >> Str; break; }
            case 10 : { exit(0); break; }
            case 17 : { exit(a0); break; }
        }
    }
    virtual void memoryAccess() {
        switch (ope) {
            case 4 : { for (int i = a0; pool[i]; i ++) fou << pool[i]; break; }
            case 8 : { for (int i = 0; i < Str.length(); i++) pool[a0 + i] = Str[i]; break; }
            case 9 : { pos = pool.distributeN(a0); break; }
        }
    }
    virtual void writeBack() {
        switch (ope) {
            case 5 : { reg[2] = Int; break; }
            case 9 : { reg[2] = pos; break; }
        }
    }
};

#endif
