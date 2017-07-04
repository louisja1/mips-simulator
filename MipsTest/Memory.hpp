#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdio>
#include "Tools.hpp"
#include <memory>

typedef vector<string> vecS;

using namespace std;

const int N = 4 * 1024 * 1024;
const int M = 1024 * 1024;

//class Memory {
//private:
    char pool[N];
    int data_cnt, text_cnt;

    int ask(int ad, int len) {
        int ret;
        memcpy(&ret, pool + ad, len);
        return ret;
    }
    int distributeN(int size) {
        int first = data_cnt;
        data_cnt += size;
        return M + first;
    }
    void addData(int type, vecS G) {
        if (type == 1) { // align
            int n = Tools::string2number(G[0]);
            int size = 1 << n;
            int res = size - data_cnt % size;
            data_cnt += res;
        }
        if (type == 2) { // ascii
            int res = G[0].length();
            for (int i = 0; i < res; i++) {
                pool[M + data_cnt + i] = G[0][i];
            }
            data_cnt += res;
        }
        if (type == 3) { // asciiz
            int res = G[0].length();
            for (int i = 0; i < res; i++) {
                pool[M + data_cnt + i] = G[0][i];
            }
            pool[M + data_cnt + res] = 0;
            data_cnt += res + 1;
        }
        if (type == 4) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                memcpy(pool + M + data_cnt, &ret, 1);
                data_cnt += 1;
            }
        }
        if (type == 5) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                memcpy(pool + M + data_cnt, &ret, 2);
                data_cnt += 2;
            }
        }
        if (type == 6) {
            for (int i = 0; i < G.size(); i++) {
                int ret = Tools::string2number(G[i]);
                memcpy(pool + M + data_cnt, &ret, 4);
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

#endif
