#include <iostream>
#include <json/json.h>
#include <getopt.h>
#include <fstream>
#include "Finder.h"
#include "Simulate.h"

using std::string;
using std::vector;
using Json::Reader;
using Json::Writer;
using std::ifstream;
using std::cout;
using std::endl;

using namespace icfp2015;

int main(int argc, char **argv) {
    vector<string> ofPower;
    int opt;
    long memLimit;
    long timeLimit;
    string fName;
    Json::Reader x;
    Json::Value root;
    bool printField = false, printUnits = false, showSol = false;

    while ((opt = getopt(argc, argv, "f:t:m:p:FUS")) >= 0) {
        switch (opt) {
            case 'F':
                printField = true;
                break;
            case 'U':
                printUnits = true;
                break;
            case 'S':
                showSol = true;
                break;
            case 'f':
                fName = optarg;
                break;
            case 't':
                timeLimit = strtol(optarg, NULL, 10);
                break;
            case 'm':
                memLimit = strtol(optarg, NULL, 10);
                break;
            case 'p':
                ofPower.push_back(optarg);
                break;
            default:
                if (opt > 0) {
                    printf("unidentified arg '%c'\n", (char) opt);
                } else {
                    printf("no args?\n");
                }
                return 1;
        }
    }

    {
        ifstream file(fName);
        x.parse(file, root, false);
    }

    Field f(root);
    if (printField)
        f.print();

    Units u(root);
    if (printUnits) {
        for (int i = 0; i < u.num(); ++i) {
            Unit tmp = u[i];
            printf("------- unit %i\n", i);
            for (int j = tmp.maxRotate(); j >= 0; --j) {
                tmp.print();
                tmp.rotate(true);
            }
        }
    }

    RNGSeeds s(root);
    long id = root["id"].asInt64();
    int maxUnits = root["sourceLength"].asInt();

    for (RNG &gen : s.list) {
        long seed = gen.seed();
        Simulate sim(f, u, gen, maxUnits);

        for (int i = 0; i < maxUnits; ++i) {

            if (!sim.nextUnit()) break;
            if (printField)
                f.print();
            while (sim.step(Actions::MoveE, true)) {
                sim.step(Actions::MoveE);
                if (printField)
                    f.print();
            }
            while (true) {
                if (sim.step(Actions::MoveSE, true)) {
                    sim.step(Actions::MoveSE);
                } else if (sim.step(Actions::MoveSW, true)) {
                    sim.step(Actions::MoveSW);
                } else {
                    break;
                }
                if (printField)
                    f.print();
            }

            while (sim.step(Actions::MoveW)) {
                if (printField)
                    f.print();
            }
            //sim.step(Actions::MoveW);
            printf("-------------- Step %i complete\n", i);
        }
        //Solver c(sim, f);
        //Solution sol = c.getSol();
        //Finder k(sol, sim, ofPower);
        //k.PrintSolution(id, seed);
        return 0;
    }

    return 0;
}