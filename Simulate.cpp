//
// Created by botanic on 8/7/15.
//

#include "Simulate.h"

bool icfp2015::Simulate::nextUnit() {
    Unit nextUnit(units[gen()]);
    uY = 0;
    uX = (field.width() - nextUnit.width()) / 2;
    if (!nextUnit.Check(field, uX, uY)) {
        return false;
    }
    nextUnit.Apply(field, uX, uY);
    curUnit = nextUnit;
    return true;
}

bool icfp2015::Simulate::step(icfp2015::Actions a, bool verify) {
    int uXnew = uX, uYnew = uY;
    Unit newUnit = curUnit;
    switch (a) {
        case Actions::MoveW:
            --uXnew;
            break;
        case Actions::MoveE:
            ++uXnew;
            break;
        case Actions::MoveSE:
            ++uYnew;
            if ((uYnew & 1) == 0) ++uXnew;
            break;
        case Actions::MoveSW:
            ++uYnew;
            if ((uYnew & 1) == 1) --uXnew;
            break;
        case Actions::TurnCW:
            newUnit.rotate(true);
            break;
        case Actions::TurnCCW:
            newUnit.rotate(false);
            break;
    }
    if (newUnit.Check(field, uXnew, uYnew)) { // can be placed
        if (!verify) {
            curUnit.Apply(field, uX, uY, true); // erase
            curUnit = newUnit;
            uX = uXnew;
            uY = uYnew;
            curUnit.Apply(field, uX, uY); // draw
        }
        return true;
    } else {
        if (!verify) {
            curUnit.Lock(field, uX, uY); // lock
            field.simplify();
        }
        return false;
    }
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
