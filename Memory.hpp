
#define MEMORY_HPP

#include <cstdio>
#include "Tools.hpp"

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
        int first = text_cnt;
        text_cnt += size;
        return first;
    }
    void addData(int type, vecS G) {
        if (type == 1) { // align
            int n = Tools::string2number(G[0]);
            int size = 1 << n;
            int res = size - text_cnt % size;
            text_cnt += res;
        }
        if (type == 2) { // ascii
            G[0] = getRealString(G[0]);
            int res = G[0].length();
            for (int i = 0; i < res; i++) {
                a[M + text_cnt + i] = G[0][i];
            }
            text_cnt += res;
        }
        if (type == 3) { // asciiz
            G[0] = Tools::getRealString(G[0]);
            int res = G[0].length();
            for (int i = 0; i < res; i++) {
                a[M + text_cnt + i] = G[0][i];
            }
            a[M + text_cnt + res] = '\0';
            text_cnt += res + 1;
        }
        if (type == 4) {
            for (int i = 0; i < G.size(); i++) {
                a[M + text_cnt] = Tools::string2number(G[i]);
                text_cnt += 1;
            }
        }
        if (type == 5) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                a[M + text_cnt] = ret / 256;
                a[M + text_cnt + 1] = ret % 256;
                text_cnt += 2;
            }
        }
        if (type == 6) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                a[M + text_cnt] = ret / 16777216;
                a[M + text_cnt + 1] = ret / 65536 % 256;
                a[M + texc_cnt + 2] = ret / 256 % 256;
                a[M + text_cnt + 3] = ret % 256;
                text_cnt += 4;
            }
        }
        if (type == 7) {
            int ret = Tools::string2number(G[i]);
            text_cnt += ret;
        }
    }
};

#endif
