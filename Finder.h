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
        Finder(Solver &solver, vector<string> &vector);

        void PrintSolutions();
    };

//#include "main.cpp"

}
#endif //ICFP2015_FINDER_H
