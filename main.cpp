#include <iostream>
#include <json/json.h>
#include <getopt.h>
#include <fstream>
#include "Finder.h"

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

    while ((opt = getopt(argc, argv, "f:t:m:p:")) >= 0) {
        switch (opt) {
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

    f.print();

    return 0;
#if 0
    Units u(root);
    RNGSeeds s(root);
    long id = root["id"].asInt64();
    for (RNG &gen : s.list) {
        long seed = gen.seed();

        f.reset();
        Solver c(gen, f, u);

        Finder k(c, f, u, ofPower);

        k.PrintSolution(id, seed);
    }

    return 0;
#endif
}