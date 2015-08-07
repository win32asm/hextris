#include <iostream>
#include <json/json.h>
#include <getopt.h>
#include <fstream>
#include "units.h"
#include "Finder.h"

using std::string;
using std::vector;
using Json::Reader;
using Json::Writer;
using std::ifstream;
using std::cout;

using namespace icfp2015;

int main(int argc, char **argv) {
    vector<string> ofPower;
    int opt;
    long memLimit;
    long timeLimit;
    string fName;
    Json::Reader x;
    Json::Value root;

    while ((opt = getopt(argc, argv, "f:t:m:p:"))) {
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
                printf("unidentified arg '%c'\n", (char) opt);
                return 1;
        }
    }

    {
        ifstream file(fName);
        x.parse(file, root, false);
    }

    Field f(root);
    Units u(root);
    Solver c(root, f, u);
    Finder k(c, ofPower);

    k.PrintSolutions();

    return 0;
}