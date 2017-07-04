#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <cstdio>
#include <string>

using namespace std;

extern const int N;

class Register {
public:
    int regist[34];

    Register() {
        for (int i = 0; i < 34; i++) {
            regist[i] = 0;
        }
        regist[29] = N - 1;
    }
    ~Register() {}
    int & operator[](const int &key) {
        return regist[key];
    }
    const int & operator[](const int &key) const {
        return regist[key];
    }
    int getid(string st) {
        int len = st.length();
        int id = 0;
        bool allnumber = true;
        for (int i = 0; i < len; i++)
            if (st[i] < '0' || st[i] > '9') {
                allnumber = false;
                break;
        }
        if (allnumber) {
            for (int i = 0; i < len; i++) {
                id = id * 10 + st[i] - '0';
            }
            if (id > 31) {
                return -1;
            }
            return id;
        }
        if (st == "zero") return 0;
        if (st == "at") return 1;
        if (st == "v0") return 2;
        if (st == "v1") return 3;
        if (st == "a0") return 4;
        if (st == "a1") return 5;
        if (st == "a2") return 6;
        if (st == "a3") return 7;
        if (st == "t0") return 8;
        if (st == "t1") return 9;
        if (st == "t2") return 10;
        if (st == "t3") return 11;
        if (st == "t4") return 12;
        if (st == "t5") return 13;
        if (st == "t6") return 14;
        if (st == "t7") return 15;
        if (st == "s0") return 16;
        if (st == "s1") return 17;
        if (st == "s2") return 18;
        if (st == "s3") return 19;
        if (st == "s4") return 20;
        if (st == "s5") return 21;
        if (st == "s6") return 22;
        if (st == "s7") return 23;
        if (st == "t8") return 24;
        if (st == "t9") return 25;
        if (st == "k0") return 26;
        if (st == "k1") return 27;
        if (st == "gp") return 28;
        if (st == "sp") return 29;
        if (st == "s8") return 30;
        if (st == "fp") return 30;
        if (st == "ra") return 31;
        if (st == "lo") return 32;
        if (st == "hi") return 33;
        return -1;
    }
};

#endif
