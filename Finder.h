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

    struct wordInfo {
        vector<Actions> actions;
        string wordOfPower;
        string prepared;
    };

// power word finder
    class Finder {
        Solution sol;
        vector<wordInfo> words;

    public:
#define allCases(a, b, c, d, e, f, g) case a: case b: case c: case d: case e: case f: newWord.prepared+=a; newWord.actions.push_back(g); break

        Finder(vector<string> ofPower) {
            // convert
            for (string &line:ofPower) {
                wordInfo newWord;
                newWord.wordOfPower = line;

                for (int i = 0; i < line.size(); ++i) {
                    switch (line[i]) {
                        allCases('p', '\'', '!', '.', '0', '3', Actions::MoveW);
                        allCases('b', 'c', 'e', 'f', 'y', '2', Actions::MoveE);
                        allCases('a', 'g', 'h', 'i', 'j', '4', Actions::MoveSW);
                        allCases('l', 'm', 'n', 'o', ' ', '5', Actions::MoveSE);
                        allCases('d', 'q', 'r', 'v', 'z', '1', Actions::TurnCW);
                        allCases('k', 's', 't', 'u', 'w', 'x', Actions::TurnCCW);
                        case '"':
                        case '\n':
                        case '\r':
                        case '\t':
                            // any processing?
                            break;
                        default:
                            break;
                    }
                }
                words.push_back(newWord);
            }
        };
#undef allCases

        const void print() {
            for (const wordInfo &wi: words) {
                printf("Word: '%s', preprocessed: '%s', Moves: [", wi.wordOfPower.c_str(), wi.prepared.c_str());
                for (Actions a:wi.actions) {
                    switch (a) {
                        case Actions::MoveW:
                            printf("W ");
                            break;
                        case Actions::MoveE:
                            printf("E ");
                            break;
                        case Actions::MoveSW:
                            printf("SW ");
                            break;
                        case Actions::MoveSE:
                            printf("SE ");
                            break;
                        case Actions::TurnCW:
                            printf("CW ");
                            break;
                        case Actions::TurnCCW:
                            printf("CC ");
                            break;
                    }
                }
                printf("\n");
            }
        }

        void FormatSolution(const Solution &moves, long id, long seed, Json::Value &root) {
            string sLine;
            for (Actions a:moves.code) {
                switch (a) {
                    case Actions::MoveW:
                        sLine += 'p';
                        break;
                    case Actions::MoveE:
                        sLine += 'b';
                        break;
                    case Actions::MoveSW:
                        sLine += 'a';
                        break;
                    case Actions::MoveSE:
                        sLine += 'l';
                        break;
                    case Actions::TurnCW:
                        sLine += 'd';
                        break;
                    case Actions::TurnCCW:
                        sLine += 'k';
                        break;
                }
            }
            for (const wordInfo &wi: words) {
                string::size_type idx = 0;
                while ((idx = sLine.find(wi.prepared, idx)) != string::npos) {
                    sLine.replace(idx, wi.prepared.size(), wi.wordOfPower);
                    idx += wi.prepared.size();
                }
            }

            Json::Value sub(Json::objectValue);
            sub["problemId"] = Json::Value::Int(id);
            sub["seed"] = Json::Value::Int(seed);
            sub["tag"] = string("B@OBab v1.0");
            sub["solution"] = sLine;
            root.append(sub);
        }
    };

//#include "main.cpp"

}
#endif //ICFP2015_FINDER_H
