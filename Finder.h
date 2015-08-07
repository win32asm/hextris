//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_FINDER_H
#define ICFP2015_FINDER_H

#include <string>
#include <vector>
#include "Solver.h"

namespace icfp2015 {
    using std::vector;
    using std::string;

// power word finder
    class Finder {

    public:
        Finder(Solution &solver, RNG &gen, Field &fld, Units &units, vector<string> ofPower) { };

        void PrintSolution(long i, long i1);
    };

//#include "main.cpp"

}
#endif //ICFP2015_FINDER_H
