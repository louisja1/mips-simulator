#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdio>
#include "Tools.hpp"

typedef vector<string> vecS;

using namespace std;

const int N = 4 * 1024 * 1024;
const int M = 1024 * 1024;

class Memory {
private:
    char a[N];
    int data_cnt, text_cnt;
public:
    Memory() {
        for (int i = 0; i < N; i++) {
            a[i] = 0;
        }
        data_cnt = 0;
        text_cnt = 0;
    }
    ~Memory() {}
    char & operator[](const int &key) {
        return a[key];
    }
    const char & operator[](const int &key) const {
        return a[key];
    }
    int distribute(bool isdata) {
        if (isdata) {
            data_cnt ++;
            return M + data_cnt - 1;
        } else {
            text_cnt ++;
            return text_cnt - 1;
        }
    }
    int distributeN(int size) {
        int first = data_cnt;
        data_cnt += size;
        return first;
    }
    void addData(int type, vecS & G) {
        if (type == 1) { // align
            int n = Tools::string2number(G[0]);
            int size = 1 << n;
            int res = size - data_cnt % size;
            data_cnt += res;
        }
        if (type == 2) { // ascii
            G[0] = Tools::getRealString(G[0]);
            int res = G[0].length();
            for (int i = 0; i < res; i++) {
                a[M + data_cnt + i] = G[0][i];
            }
            data_cnt += res;
        }
        if (type == 3) { // asciiz
            G[0] = Tools::getRealString(G[0]);
            int res = G[0].length();
            for (int i = 0; i < res; i++) {
                a[M + data_cnt + i] = G[0][i];
            }
            a[M + data_cnt + res] = '\0';
            data_cnt += res + 1;
        }
        if (type == 4) {
            for (int i = 0; i < G.size(); i++) {
                a[M + data_cnt] = Tools::string2number(G[i]);
                data_cnt += 1;
            }
        }
        if (type == 5) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                a[M + data_cnt] = ret / 256;
                a[M + data_cnt + 1] = ret % 256;
                data_cnt += 2;
            }
        }
        if (type == 6) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                a[M + data_cnt] = ret / 16777216;
                a[M + data_cnt + 1] = ret / 65536 % 256;
                a[M + data_cnt + 2] = ret / 256 % 256;
                a[M + data_cnt + 3] = ret % 256;
                data_cnt += 4;
            }
        }
        if (type == 7) {
            int ret = Tools::string2number(G[0]);
            data_cnt += ret;
        }
    }
    int dataCnt() {
        return M + data_cnt;
    }
    int loadByte(int pos) {
        int res = 0;
        for (int i = 0; i < 1; i++) {
            res = (res << 8) + a[pos + i];
        }
        return res;
    }
    int loadHalfword(int pos) {
        int res = 0;
        for (int i = 0; i < 2; i++) {
            res = (res << 8) + a[pos + i];
        }
        return res;
    }
    int loadWord(int pos) {
        int res = 0;
        for (int i = 0; i < 4; i++) {
            res = (res << 8) + a[pos + i];
        }
        return res;
    }
    void storeByte(int pos, int val) {
        int tmp = pos + 0;
        for (int i = 0; i < 1; i++) {
            a[tmp - i] = val % (1 << 8);
            val >>= 8;
        }
    }
    void storeHalfword(int pos, int val) {
        int tmp = pos + 1;
        for (int i = 0; i < 2; i++) {
            a[tmp - i] = val % (1 << 8);
            val >>= 8;
        }
    }

    void storeWord(int pos, int val) {
        int tmp = pos + 3;
        for (int i = 0; i < 4; i++) {
            a[tmp - i] = val % (1 << 8);
            val >>= 8;
        }
    }
};

#endif
