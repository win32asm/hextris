//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_SOLVER_H
#define ICFP2015_SOLVER_H

#include <json/json.h>
#include <vector>
#include "units.h"
#include "Field.h"
#include "RNGSeed.h"

namespace icfp2015 {

    using std::vector;

    enum class Actions {
        MoveE, MoveR, MoveSE, MoveSR, TurnCW, TurnCCW
    };

    class Solution {
    public:
        vector<Actions> code;
    };

// tetris solver
    class Solver {

    public:
        Solver(icfp2015::RNG &value, Field &field, Units &units);

        Solution operator()() {
            Solution s;
            return s;
        }
    };


}

#endif //ICFP2015_SOLVER_H
