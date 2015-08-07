//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_SOLVER_H
#define ICFP2015_SOLVER_H

#include <json/json.h>
#include "units.h"
#include "Field.h"

namespace icfp2015 {

// tetris solver
    class Solver {
        enum class actions {
            MoveE, MoveR, MoveSE, MoveSR, TurnCW, TurnCCW
        };
    public:
        Solver(Json::Value &value, Field &field, Units &units);
    };
}

#endif //ICFP2015_SOLVER_H
