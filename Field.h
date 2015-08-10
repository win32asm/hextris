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
#include "Logger.h"

namespace icfp2015 {
    using std::vector;
    using std::string;
// field
    class Field {
        vector<char> field;     // original state
        vector<char> curfield;  // updated state
        unsigned int wid;
        unsigned int hei;

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

        const long penalty() const {
            long penalty = 0;
            vector<int> lock;
            vector<int> free;
            for (int line = 0; line < hei; ++line) {
                bool locked = true;
                int straight = 0;
                int closed = 0;
                for (int col = 0; col < wid; ++col) {
                    if (locked ? (curfield[line * wid + col] != 0) : (curfield[line * wid + col] == 0)) {
                        ++straight;
                    } else {
                        (locked ? lock : free).push_back(straight);
                        locked = !locked;
                        straight = 1;
                    }
                    if (line != 0 && !locked) { // upper neighbors for free spaces
                        int prevIdx = (line & 1) ? col : (col - 1);
                        if (prevIdx >= 0 && curfield[(line - 1) * wid + prevIdx] != 0)
                            ++closed;
                        if (curfield[(line - 1) * wid + prevIdx + 1] != 0)
                            ++closed;
                    }
                }
                (locked ? lock : free).push_back(straight);

                penalty += (lock.size() + free.size() - 1) * 10; // each interval + 10
                penalty += 100 * closed; // each closed space + 100
                if (lock[0] != 0) penalty -= 20; // locked from right or left
                if (lock.size() > free.size()) penalty -= 20;

                if (lock.size() != 0) {
                    for (int sz:free) {
                        penalty += 50 * (wid - sz);
                    }
                }
            }
            return penalty;
        }


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

        const void print(const string &s, bool withPenalty = false) const {
            if (withPenalty) {
                glbLog() << "-----------------(" << penalty() << ") " << s.c_str() << endl;
            } else {
                glbLog() << "-----------------" << s.c_str() << endl;
            }

            for (int i = 0; i < hei; ++i) {
                if (i & 1) glbLog() << ' ';
                for (int j = 0; j < wid; ++j) {
                    glbLog() << (curfield[i * wid + j] ?: '.') << ' ';
                }
                glbLog() << endl;
            }
            glbLog() << "-----------------" << endl;
        }
    };

}

#endif //ICFP2015_FIELD_H
