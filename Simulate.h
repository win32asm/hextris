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
        Field field;
        Units units;
    public:
        Simulate(Field &f, Units &u) : field(f), units(u) { };

        bool nextUnit(long index); // returns true if figure can be placed
        bool step(Actions a); // returns true if action didn`t lock
        long score();

        long run(Solution &sol, RNG &gen); // returns score, field not reset after.
    };
}


#endif //ICFP2015_SIMULATE_H
