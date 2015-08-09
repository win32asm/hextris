//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_FIELD_H
#define ICFP2015_FIELD_H

#include <json/json.h>
#include <vector>
#include <cstring>
#include <string>
#include "Actions.h"

namespace icfp2015 {
    using std::vector;
    using std::string;
// field
    class Field {
        vector<char> field;     // original state
        vector<char> curfield;  // updated state
        unsigned int wid;
        unsigned int hei;

        const void _print() const {
            for (int i = 0; i < hei; ++i) {
                if (i & 1) printf(" ");
                for (int j = 0; j < wid; ++j) {

                    printf("%c ", curfield[i * wid + j] ?: '.');
                }
                printf("\n");
            }
            printf("-----------------\n");
        }

    public:
        Field(const Json::Value &root) {
            wid = root["width"].asUInt();
            hei = root["height"].asUInt();
            field.resize(wid * hei);
            Json::Value cells = root["filled"];
            for (const Json::Value &dot:cells) {
                int x = dot["x"].asInt();
                int y = dot["y"].asInt();
                field[x + y * wid] = '*';
            }
            curfield = field;
        }

        const unsigned width() { return wid; }

        const unsigned height() { return hei; }

        void reset() {
            curfield = field;
        }

        void clr(int x, int y) {
            curfield[wid * y + x] = 0;
        }

        void set(int x, int y, bool locked = false) {
            curfield[wid * y + x] = locked ? '*' : 'F';
        }

        const char operator()(int x, int y) {
            return curfield[wid * y + x];
        }

        int simplify() {
            int removedLines = 0;
            for (int i = 0; i < hei; ++i) {
                bool allSet = true;
                for (int j = 0; j < wid; ++j) {
                    allSet &= curfield[i * wid + j] == '*';
                }
                if (allSet) {
                    ++removedLines;
                    if (i != 0) {
                        memmove(&curfield[wid], &curfield[0], i * wid);
                    } else {
                        memset(&curfield[0], 0, wid);
                    }
                }
            }
            return removedLines;
        }

        const void print(const string &s) const {
            printf("----------------- %s\n", s.c_str());
            _print();
            printf("-----------------");
        }
    };

}

#endif //ICFP2015_FIELD_H
