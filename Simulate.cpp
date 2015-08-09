//
// Created by botanic on 8/7/15.
//

#include "Simulate.h"

using namespace icfp2015;

bool icfp2015::Simulate::nextUnit() {
    Unit nextUnit(units[gen()]);
    uY = 0;
    uX = (field.width() - nextUnit.width()) / 2 - nextUnit.disp();
    if (!nextUnit.Check(field, uX, uY)) {
        return false;
    }
    nextUnit.Apply(field, uX, uY);
    curUnit = nextUnit;
    unitPath.Reset(nextUnit);
    unitPath.Save(uX, uY, curUnit.Orient());
    return true;
}

VerifyState icfp2015::Simulate::step(Actions a, bool verify) {
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

    if (!unitPath.Verify(uXnew, uYnew, newUnit.Orient())) {
        return VerifyState::Fail;
    }

    if (newUnit.Check(field, uXnew, uYnew)) { // can be placed
        if (!verify) {
            last.code.push_back(a);
            curUnit.Apply(field, uX, uY, true); // erase
            curUnit = newUnit;
            uX = uXnew;
            uY = uYnew;
            curUnit.Apply(field, uX, uY); // draw
            unitPath.Save(uX, uY, curUnit.Orient());
        }
        return VerifyState::Pass;
    } else {
        if (!verify) {
            last.code.push_back(a);
            curUnit.Lock(field, uX, uY); // lock
            int sz = curUnit.size();
            int lines = field.simplify();
            int pts = sz + 50 * lines * (lines + 1);
            curScore += pts + (lastLines - 1) * pts / 10;
            lastLines = lines;
        }
        return VerifyState::Lock;
    }
}

long icfp2015::Simulate::run(Solution &sol) {
    /*bool needFigure = true;
    field.reset();
    gen.reset();
    last.reset();
    curScore = 0;
    lastLines = 0;

    for (Actions a : sol.code) {
        if (needFigure && !nextUnit()) {
            break;
        }
        needFigure = !step(a);
    }

    return score();*/
    return 0;
}

Solution icfp2015::Simulate::Moves() {
    return last;
}
