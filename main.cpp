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
    bool printField = false, printUnits = false, showScore = false, showPower = false, runOne = false;

    while ((opt = getopt(argc, argv, "f:t:m:p:FUSPO")) >= 0) {
        switch (opt) {
            case 'O':
                runOne = true;
                break;
            case 'P':
                showPower = true;
                break;
            case 'F':
                printField = true;
                break;
            case 'U':
                printUnits = true;
                break;
            case 'S':
                showScore = true;
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
    Finder k(ofPower);
    if (showPower) {
        k.print();
    }
    Json::Value outRoot(Json::arrayValue);

    for (RNG &gen : s.list) {
        long seed = gen.seed();
        Solver slv(gen, f, u, k.Words());

        int baseScore = (int) slv.Run(printField, maxUnits);

        int powerScore = k.FormatSolution(slv.getSol(), id, seed, outRoot);

        if (showScore) {
            printf("solution score: %i\n", baseScore + powerScore);
        }
        if (runOne)
            break;
    }

    {
        Json::FastWriter fw;
        printf("%s", fw.write(outRoot).c_str());
    }

    return 0;
}