//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_UNITS_H
#define ICFP2015_UNITS_H

#include <json/json.h>
#include <vector>
#include "Field.h"

namespace icfp2015 {
    using std::vector;
// one figure
    class Unit {
        int pX, pY;
        vector<int> xList;
        vector<int> yList;
        int xbm, ybm, xbx, ybx; // bounding box

        int orient;
    public:
        Unit() { }

        Unit(const Unit &u) : pX(u.pX), pY(u.pY), xList(u.xList), yList(u.yList),
                              xbm(u.xbm), ybm(u.ybm), xbx(u.xbx), ybx(u.ybx), orient(u.orient) { }

        Unit &operator=(const Unit &u) {
            pX = u.pX;
            pY = u.pY;
            orient = u.orient;
            xList = u.xList;
            yList = u.yList;
            xbm = u.xbm;
            ybm = u.ybm;
            xbx = u.xbx;
            ybx = u.ybx;
            return *this;
        }

        void Translate(int x, int y, int newori) {
            while (orient != newori) {
                rotate(true);
            }
            for (int i = 0; i < xList.size(); ++i) {
                xList[i] += x;
                yList[i] += y;
                if ((yList[i] & 1) == 0 && (y & 1) == 1) ++xList[i];
            }
        }

        const bool operator==(const Unit &u) const {
            bool hasPair = true;
            for (int i = 0; i < xList.size() && hasPair; ++i) {
                hasPair = false;
                for (int j = 0; j < u.xList.size(); ++j) {
                    if (xList[i] == u.xList[j] && yList[i] == u.yList[j]) {
                        hasPair = true;
                        break;
                    }
                }
            }
            return hasPair;
        }

        Unit(const Json::Value uData) {
            xbm = ybm = 1000;
            xbx = ybx = -1000;
            const Json::Value pData = uData["pivot"];
            pX = pData["x"].asInt();
            pY = pData["y"].asInt();
            for (const Json::Value &pt:uData["members"]) {
                int x = pt["x"].asInt();
                int y = pt["y"].asInt();
                xList.push_back(x);
                yList.push_back(y);
                if (xbm > x) xbm = x;
                if (xbx < x) xbx = x;
                if (ybm > y) ybm = y;
                if (ybx < y) ybx = y;
            }
            orient = 0;
        }

        void rotate(bool cw) {
            xbm = ybm = 1000;
            xbx = ybx = -1000;

            int pX3 = pX - (pY - (pY & 1)) / 2;    // pivot odd_r to 3d
            int pZ3 = pY;
            int pY3 = -pX3 - pZ3;


            for (int i = 0; i < xList.size(); ++i) {
                int xO = xList[i];
                int yO = yList[i]; // odd_r coords

                int x3 = xO - (yO - (yO & 1)) / 2;
                int z3 = yO;
                int y3 = -x3 - z3; // odd_r to 3d

                // shift by pivot pt
                x3 -= pX3;
                y3 -= pY3;
                z3 -= pZ3;

                if (cw) {  // rotate 60 deg
                    int temp = x3;
                    x3 = -z3;
                    z3 = -y3;
                    y3 = -temp;
                } else {
                    int temp = x3;
                    x3 = -y3;
                    y3 = -z3;
                    z3 = -temp;
                }

                //shift back
                x3 += pX3;
                y3 += pY3;
                z3 += pZ3;

                // 3d to odd_r
                yO = yList[i] = z3;
                xO = xList[i] = x3 + (z3 - (z3 & 1)) / 2;

                if (xbm > xO) xbm = xO; // new bounding box
                if (xbx < xO) xbx = xO;
                if (ybm > yO) ybm = yO;
                if (ybx < yO) ybx = yO;
            }
            orient += cw ? 1 : -1;
            if (orient < 0) orient += 6;
            if (orient >= 6) orient %= 6;
        }

        const bool Check(Field &f, int atx, int aty) {
            for (int i = 0; i < xList.size(); ++i) {
                int ptX = xList[i] + atx;
                int ptY = yList[i] + aty;
                if ((ptY & 1) == 0 && (aty & 1) == 1) ++ptX;
                if (ptX < 0 || ptX >= f.width() || ptY < 0 || ptY >= f.height())
                    return false;
                if (f(ptX, ptY) == '*') // old figure ('F') is OK
                    return false;
            }
            return true;
        }

        const void Apply(Field &f, int atx, int aty, bool erase = false) {
            for (int i = 0; i < xList.size(); ++i) {
                int ptX = xList[i] + atx;
                int ptY = yList[i] + aty;
                if ((ptY & 1) == 0 && (aty & 1) == 1) ++ptX;
                if (erase) {
                    f.clr(ptX, ptY);
                } else {
                    f.set(ptX, ptY);
                }
            }
        }

        const void Lock(Field &f, int atx, int aty) {
            for (int i = 0; i < xList.size(); ++i) {
                int ptX = xList[i] + atx;
                int ptY = yList[i] + aty;
                if ((ptY & 1) == 0 && (aty & 1) == 1) ++ptX;
                f.set(ptX, ptY, true);
            }
        }

        const void print() {
            vector<char> fld;
            printf("-----------------(%i, %i)\n", pX, pY);
            int wid = (xbx - xbm + 2);
            int hei = (ybx - ybm + 2 + (ybm & 1));
            fld.resize(wid * hei);
            for (int i = 0; i < xList.size(); ++i) {
                int xO = xList[i] - xbm;
                int yO = yList[i] - ybm + (ybm & 1); // move by even N of rows
                int idx = xO + yO * wid;
                fld[idx] = '*';
                printf("(%i, %i),", xO + xbm, yO + ybm);
            }
            printf("\n");

            for (int i = 0; i < hei; ++i) {
                if (i & 1) printf(" ");
                for (int j = 0; j < wid; ++j) {
                    printf("%c ", fld[i * wid + j] ?: '.');
                }
                printf("\n");
            }
        }

        int width() const;

        int size() const;

        const int Orient() const { return orient; }
    };

// set of figures
    class Units {
        vector<Unit> list;
    public:
        const size_t num() { return list.size(); }

        Units(const Json::Value &root);

        const Unit &operator[](int i) const { return list[i % list.size()]; }
    };

    class Path {
        vector<Unit> uPath;
        Unit base;
    public:

        void Reset(const Unit &newUnit) {
            base = newUnit;
            uPath.clear();
        }

        bool Verify(int x, int y, int ori) {
            Unit next(base);
            next.Translate(x, y, ori);
            for (const Unit &u:uPath) {
                if (u == next)
                    return false;
            }
            return true;
        }

        void Save(int x, int y, int ori) {
            Unit next(base);
            next.Translate(x, y, ori);
            uPath.push_back(next);
        }
    };

}


#endif //ICFP2015_UNITS_H
