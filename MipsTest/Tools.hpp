#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <cstdio>
#include <string>
#include "Register.hpp"

using namespace std;

extern Register reg;

class Tools {
public:
    static int getRegister(string _reg) {
        return reg.getid(_reg.substr(1, _reg.length() - 1));
    }
    static int string2number(string st) {
        int len = st.length();
        if (len == 0) return 0;
        if (st[0] == '-') {
            int res = 0;
            for (int i = 1; i < len; i++) {
                res = res * 10 + st[i] - '0';
            }
            return res * (-1);
        } else {
            int res = 0;
            for (int i = 0; i < len; i++) {
                res = res * 10 + st[i] - '0';
            }
            return res;
        }
    }
    static bool allnumber(string st) {
        for (int i = 0; i < st.length(); i++) {
            if (i == 0 && st[i] == '-') continue;
            if (st[i] > '9' || st[i] < '0') {
                return false;
            }
        }
        return true;
    }
    static string getRealString(string str) {
        string ans = "";
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '\\' && i + 1 < str.length()) {
                char nxt = str[i + 1];
                switch (nxt) {
                    case 'n' : {
                        ans += '\n';
                        break;
                    }
                    case 'r' : {
                        ans += '\r';
                        break;
                    }
                    case 't' : {
                        ans += '\t';
                        break;
                    }
                    case '0' : {
                        ans += '\0';
                        break;
                    }
                    case '\\' : {
                        ans += '\\';
                        break;
                    }
                    case '\"' : {
                        ans += '\"';
                        break;
                    }
                    case '\'' : {
                        ans += '\'';
                        break;
                    }
                }
                i ++;
            } else {
                ans += str[i];
            }
        }
        return ans;
    }
};

#endif
