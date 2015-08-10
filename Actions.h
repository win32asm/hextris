//
// Created by botanic on 8/8/15.
//

#ifndef ICFP2015_ACTIONS_H
#define ICFP2015_ACTIONS_H

#include <string>
#include <vector>

namespace icfp2015 {

    using std::vector;
    using std::string;

    enum PrintData {
        PRINT_FIELD = 1,
        PRINT_START,
        PRINT_STEP,
        PRINT_ALL,
        PRINT_DEBUG
    };

    enum class Actions {
        MoveE, MoveW, MoveSE, MoveSW, TurnCW, TurnCCW, NoAction
    };

    static const char *ToString(Actions a) {
        switch (a) {
            case Actions::MoveW:
                return " W";
            case Actions::MoveE:
                return " E";
            case Actions::MoveSW:
                return " SW";
            case Actions::MoveSE:
                return " SE";
            case Actions::TurnCW:
                return " CW";
            case Actions::TurnCCW:
                return " CCW";
            default:
                return " NO";
        }
    }

    class WordInfo {
    public:
        std::vector<Actions> actions;
        std::string wordOfPower;
        std::string prepared;
    };

    class Solution {
    public:
        std::vector<Actions> code;

        void reset() {
            code.clear();
        }
    };

}

#endif //ICFP2015_ACTIONS_H
