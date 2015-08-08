//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_SIMULATE_H
#define ICFP2015_SIMULATE_H

#include "units.h"
#include "Field.h"
#include "Solver.h"

namespace icfp2015 {

    class Simulate {
        Field &field; // shared field
        const Units &units; // shared const
        RNG gen;

        Solution last;
        Unit curUnit;
        int uX, uY; // unit coords
        const int nUnits;
        int nCurUnit;
    public:
        Simulate(Field &f, const Units &u, RNG &g, int maxU) : field(f), units(u), gen(g), nCurUnit(0),
                                                               nUnits(maxU) { };

        bool nextUnit(); // returns true if figure can be placed
        bool step(Actions a, bool verify = false); // returns true if action didn`t lock
        long score();

        long run(Solution &sol); // returns score, field not reset after.

        Solution getLast() { return last; }
    };
}


#endif //ICFP2015_SIMULATE_H
