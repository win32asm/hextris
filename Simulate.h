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
    public:
        Simulate(Field &f, const Units &u, RNG &g) : field(f), units(u), gen(g) { };

        bool nextUnit(); // returns true if figure can be placed
        bool step(Actions a); // returns true if action didn`t lock
        long score();

        long run(Solution &sol); // returns score, field not reset after.

        Solution getLast() { return last; }
    };
}


#endif //ICFP2015_SIMULATE_H
