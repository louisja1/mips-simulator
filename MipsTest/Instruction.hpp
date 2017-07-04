#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdio>
#include <string>
#include <memory>

typedef vector<string> vecS;
typedef unsigned int UINT;
typedef unsigned long long ULL;
typedef long long LL;

extern map<string, int> labelLineID;
extern map<string, int> labelMemoryID;
//extern Memory pool;

using namespace std;

extern int Cur;

class Instruction {
public:
    Instruction() {}
    virtual ~Instruction() {}
    virtual Instruction* copy() { return new Instruction(*this); }
    virtual void dataPrepare() {}
    virtual void execution() {}
    virtual void memoryAccess() {}
    virtual void writeBack() {}
};

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
        //cout << imm2 << endl;
    }
    virtual ~Caculation() {}
    virtual Instruction* copy() { return new Caculation(*this); }
    virtual void dataPrepare() {
        if (rsrc1 != -1) imm1 = reg[rsrc1];
        if (rsrc2 != -1) imm2 = reg[rsrc2];
    }
    virtual void writeBack() {
        reg[rdest] = result;
    }
};

class Add : public Caculation {
public:
    bool unsign;
    Add(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {}
    virtual Instruction* copy() { return new Add(*this); }
    virtual void execution() { result = imm1 + imm2; }
};

class Sub : public Caculation {
public:
    bool unsign;
    Sub(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {}
    virtual Instruction* copy() { return new Sub(*this); }
    virtual void execution() { result = imm1 - imm2; }
};

class Mul : public Caculation {
public:
    bool unsign;
    bool isempty;
    LL ans;
    Mul(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {
            isempty = (_rsrc3 == "");
        }
    virtual Instruction* copy() { return new Mul(*this); }
    virtual void dataPrepare() {
        if (isempty) {
            imm2 = imm1;
            if (rsrc1 != -1) imm2 = reg[rsrc1];
            imm1 = reg[rdest];
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
            reg[32] = ans % (1LL << 32);
            reg[33] = ans >> 32;
        } else {
            reg[rdest] = ans;
        }
    }
};

class Div : public Caculation {
public:
    bool unsign;
    bool isempty;
    int quo, rem;
    Div(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {
            isempty = (_rsrc3 == "");
        }
    virtual Instruction* copy() { return new Div(*this); }
    virtual void dataPrepare() {
        if (isempty) {
            imm2 = imm1;
            if (rsrc1 != -1) imm2 = reg[rsrc1];
            imm1 = reg[rdest];
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
            reg[32] = quo;
            reg[33] = rem;
        } else {
            reg[rdest] = quo;
        }
    }
};

class Xor : public Caculation {
public:
    bool unsign;
    Xor(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {}
    virtual Instruction* copy() { return new Xor(*this); }
    virtual void execution() { result = imm1 ^ imm2; }
};

class Neg : public Caculation {
public:
    bool unsign;
    Neg(const string &_rsrc1, const string &_rsrc2, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, ""), unsign(_unsign) {}
    virtual Instruction* copy() { return new Neg(*this); }
    virtual void execution() { result = -imm1; }
};

class Rem : public Caculation {
public:
    bool unsign;
    Rem(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3, bool _unsign)
        : Caculation(_rsrc1, _rsrc2, _rsrc3), unsign(_unsign) {}
    virtual Instruction* copy() { return new Rem(*this); }
    virtual void execution() {
        if (unsign) {
            result = UINT(imm1) % UINT(imm2);
        } else {
            result = imm1 % imm2;
        }
    }
};

class Seq : public Caculation {
public:
    bool unsign;
    Seq(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : Caculation(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Seq(*this); }
    virtual void execution() { result = (imm1 == imm2); }
};

class Sge : public Caculation {
public:
    bool unsign;
    Sge(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : Caculation(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Sge(*this); }
    virtual void execution() { result = (imm1 >= imm2); }
};

class Sgt : public Caculation {
public:
    bool unsign;
    Sgt(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : Caculation(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Sgt(*this); }
    virtual void execution() { result = (imm1 > imm2); }
};

class Sle : public Caculation {
public:
    bool unsign;
    Sle(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : Caculation(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Sle(*this); }
    virtual void execution() { result = (imm1 <= imm2); }
};

class Slt : public Caculation {
public:
    bool unsign;
    Slt(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : Caculation(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Slt(*this); }
    virtual void execution() { result = (imm1 < imm2); }
};

class Sne : public Caculation {
public:
    bool unsign;
    Sne(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : Caculation(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Sne(*this); }
    virtual void execution() { result = (imm1 != imm2); }
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
    virtual Instruction* copy() { return new BranchJump(*this); }
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

class Beq : public BranchJump {
public:
    Beq(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : BranchJump(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Beq(*this); }
    virtual void execution() { flag = (imm1 == imm2); }
};

class Bne : public BranchJump {
public:
    Bne(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : BranchJump(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Bne(*this); }
    virtual void execution() { flag = (imm1 != imm2); }
};

class Bge : public BranchJump {
public:
    Bge(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : BranchJump(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Bge(*this); }
    virtual void execution() { flag = (imm1 >= imm2); }
};

class Ble : public BranchJump {
public:
    Ble(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : BranchJump(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Ble(*this); }
    virtual void execution() { flag = (imm1 <= imm2); }
};

class Bgt : public BranchJump {
public:
    Bgt(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : BranchJump(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Bgt(*this); }
    virtual void execution() { flag = (imm1 > imm2); }
};

class Blt : public BranchJump {
public:
    Blt(const string &_rsrc1, const string &_rsrc2, const string &_rsrc3)
        : BranchJump(_rsrc1, _rsrc2, _rsrc3) {}
    virtual Instruction* copy() { return new Blt(*this); }
    virtual void execution() { flag = (imm1 < imm2); }
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
            id = labelLineID[_rdest];
        }
    }
    virtual ~Goto() {}
    virtual Instruction* copy() { return new Goto(*this); }
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
            id = labelLineID[_rsrc];
        }
    }
    virtual ~Jal() {}
    virtual Instruction* copy() { return new Jal(*this); }
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
    int rdest, rsrc;
    string address;
    int id, storage, offset;
    Load(const string &_rdest, const string &_address)
        : Instruction(), rdest(Tools::getRegister(_rdest)), address(_address), id(-1), storage(0), offset(0) {
            if (labelMemoryID.count(address) > 0) {
                id = labelMemoryID[address];
            } else {
                int le, ri;
                for (int i = 0; i < address.length(); i++) {
                    if (address[i] == '(') le = i;
                    if (address[i] == ')') ri = i;
                }
                rsrc = Tools::getRegister(address.substr(le + 1, ri - le - 1));
                offset = Tools::string2number(address.substr(0, le));
            }
        }
    virtual ~Load() {}
    virtual Instruction* copy() { return new Load(*this); }
    virtual void dataPrepare() {
        if (id == -1) {
            id = reg[rsrc];
        }
    }
    virtual void execution() {
        id += offset;
    }
    virtual void memoryAccess() {}
    virtual void writeBack() {
        reg[rdest] = storage;
    }
};

class La : public Load {
public:
    La(const string &_rsrc1, const string &_rsrc2) : Load(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new La(*this); }
    virtual void writeBack() { reg[rdest] = id; }
};

class Lb : public Load {
public:
    Lb(const string &_rsrc1, const string &_rsrc2) : Load(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new Lb(*this); }
    virtual void memoryAccess() { memcpy(&storage, pool + id, 1); }
};

class Lh : public Load {
public:
    Lh(const string &_rsrc1, const string &_rsrc2) : Load(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new Lh(*this); }
    virtual void memoryAccess() { memcpy(&storage, pool + id, 2); }
};

class Lw : public Load {
public:
    Lw(const string &_rsrc1, const string &_rsrc2) : Load(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new Lw(*this); }
    virtual void memoryAccess() { memcpy(&storage, pool + id, 4); }
};

//store
class Store : public Instruction {
public:
    int rdest, rsrc;
    string address;
    int id, value, offset;
    Store(const string &_rdest, const string &_address)
        : Instruction(), rdest(Tools::getRegister(_rdest)), address(_address), id(-1), value(0), offset(0) {
            if (labelMemoryID.count(address) > 0) {
                id = labelMemoryID[address];
            } else {
                int le, ri;
                for (int i = 0; i < address.length(); i++) {
                    if (address[i] == '(') le = i;
                    if (address[i] == ')') ri = i;
                }
                rsrc = Tools::getRegister(address.substr(le + 1, ri - le - 1));
                offset = Tools::string2number(address.substr(0, le));
            }
        }
    virtual ~Store() {}
    virtual Instruction* copy() { return new Store(*this); }
    virtual void dataPrepare() {
        value = reg[rdest];
        if (id == -1) {
            id = reg[rsrc];
        }
    }
    virtual void execution() {
        id += offset;
    }
    virtual void memoryAccess() {}
};

class Sb : public Store {
public:
    Sb(const string &_rsrc1, const string &_rsrc2) : Store(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new Sb(*this); }
    virtual void memoryAccess() { memcpy(pool + id, &value, 1); }
};

class Sh : public Store {
public:
    Sh(const string &_rsrc1, const string &_rsrc2) : Store(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new Sh(*this); }
    virtual void memoryAccess() { memcpy(pool + id, &value, 2);  }
};

class Sw : public Store {
public:
    Sw(const string &_rsrc1, const string &_rsrc2) : Store(_rsrc1, _rsrc2) {}
    virtual Instruction* copy() { return new Sw(*this); }
    virtual void memoryAccess() { memcpy(pool + id, &value, 4);  }
};

//move
class Move : public Instruction {
public:
    int rdest, rsrc, imm;
    Move(const string &_rdest, const string &_rsrc) : Instruction() {
        rdest = Tools::getRegister(_rdest);
        if (Tools::allnumber(_rsrc)) {
            imm = Tools::string2number(_rsrc);
            rsrc = -1;
        } else {
            rsrc = Tools::getRegister(_rsrc);
        }
    }
    virtual ~Move() {}
    virtual Instruction* copy() { return new Move(*this); }
    virtual void dataPrepare() {
        if (rsrc != -1) imm = reg[rsrc];
    }
    virtual void writeBack() {
        reg[rdest] = imm;
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
    Syscall(istream &_fda, ostream &_fou) : fda(_fda), fou(_fou) {}
    virtual ~Syscall() {}
    virtual Instruction* copy() { return new Syscall(*this); }
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
            case 5 : { fda >> Int; break; }
            case 8 : { fda >> Str; break; }
            case 10 : { fou.flush(); exit(0); break; }
            case 17 : { fou.flush(); exit(a0); break; }
        }
    }
    virtual void memoryAccess() {
        switch (ope) {
            case 4 : { for (int i = a0; pool[i]; i ++) fou << pool[i]; break; }
            case 8 : { for (int i = 0; i < Str.length(); i++) pool[a0 + i] = Str[i]; break; }
            case 9 : { pos = distributeN(a0); break; }
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
