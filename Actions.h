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
        PRINT_ALL
    };

    enum class Actions {
        MoveE, MoveW, MoveSE, MoveSW, TurnCW, TurnCCW
    };

    class WordInfo {
    public:
        std::vector<Actions> actions;
        std::string wordOfPower;
        std::string prepared;
    };

    class Solution {
    public:
        std::vector<Actions> code;
        std::vector<long> scores;

        void reset() {
            code.clear();
            scores.clear();
        }
    };

}

#endif //ICFP2015_ACTIONS_H
