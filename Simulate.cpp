//
// Created by botanic on 8/7/15.
//

#include "Simulate.h"

bool icfp2015::Simulate::nextUnit(long index) {
    return false;
}

bool icfp2015::Simulate::step(icfp2015::Actions a) {
    return false;
}

long icfp2015::Simulate::score() {
    return 0;
}

long icfp2015::Simulate::run(icfp2015::Solution &sol, RNG &gen) {
    bool needFigure = true;
    field.reset();
    gen.reset();

    for (Actions a : sol.code) {
        if (needFigure && !nextUnit(gen())) {
            break;
        }

        needFigure = !step(a);
    }

    return score();
}
