//
// Created by botanic on 8/7/15.
//

#include "Simulate.h"

bool icfp2015::Simulate::nextUnit() {
    return false;
}

bool icfp2015::Simulate::step(icfp2015::Actions a) {


    return false;
}

long icfp2015::Simulate::score() {
    return 0;
}

long icfp2015::Simulate::run(icfp2015::Solution &sol) {
    bool needFigure = true;
    field.reset();
    gen.reset();
    last.reset();

    for (Actions a : sol.code) {
        if (needFigure && !nextUnit()) {
            break;
        }

        needFigure = !step(a);
    }

    return score();
}
