//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_SIMULATE_H
#define ICFP2015_SIMULATE_H

#include "Actions.h"
#include "RNGSeed.h"
#include "units.h"
#include "Field.h"

namespace icfp2015 {

    enum class VerifyState {
        Pass, Fail, Lock
    };

    class Simulate {
        Field field;
        const Units &units; // shared const
        RNG gen;

        Solution last;
        Path unitPath;
        Unit curUnit;
        int uX, uY; // unit coords
        const int nUnits;
        int nCurUnit;

        int curScore;
        int lastLines;
    public:
        Simulate(const Field &f, const Units &u, RNG &g, int maxU) : field(f), units(u), gen(g), nCurUnit(0),
                                                                     curScore(0),
                                                               lastLines(0), nUnits(maxU) { };

        bool nextUnit(); // returns true if figure can be == is placed
        VerifyState step(Actions a, bool verify = false); // returns true if action didn`t lock
        const long score() { return curScore; }

        long run(Solution &sol); // returns score, field not reset after.

        const void print(const string &note) const { field.print(note); }

        const long penalty() const { return field.penalty(); }

        const void print() const {
            char buf[120];
            snprintf(buf, 120, "@(%i, %i %% %i) after", uX, uY, curUnit.Orient());
            string rslt(buf);
            rslt += ToString(last.code.back());
            field.print(rslt, true);
        }

        const Solution getLast() const { return last; }

        Solution Moves();
    };
}


#endif //ICFP2015_SIMULATE_H
