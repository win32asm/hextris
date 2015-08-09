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

    enum class Actions {
        MoveE, MoveW, MoveSE, MoveSW, TurnCW, TurnCCW
    };

    class WordInfo {
    public:
        std::vector<Actions> actions;
        std::string wordOfPower;
        std::string prepared;
    };

    class Path {
        vector<int> xPath;
        vector<int> yPath;
        vector<int> orient;

    public:

        void Reset() {
            xPath.clear();
            yPath.clear();
            orient.clear();
        }

        bool Verify(int x, int y, int ori) {
            for (int i = 0; i < xPath.size(); ++i) {
                if (xPath[i] == x && yPath[i] == y && orient[i] == ori)
                    return false;
            }
            return true;
        }

        void Save(int x, int y, int ori) {
            xPath.push_back(x);
            yPath.push_back(y);
            orient.push_back(ori);
        }
    };

    class Solution {
    public:
        std::vector<Actions> code;

        void reset() { code.clear(); }
    };

}

#endif //ICFP2015_ACTIONS_H
