//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_FINDER_H
#define ICFP2015_FINDER_H

#include <string>
#include <vector>
#include <algorithm>
#include "Actions.h"
#include "Solver.h"

namespace icfp2015 {
    using std::vector;
    using std::string;
    using std::sort;

// power word finder
    class Finder {
        Solution sol;
        vector<WordInfo> words;

    public:
#define allCases(a, b, c, d, e, f, g) case a: case b: case c: case d: case e: case f: newWord.prepared+=a; newWord.actions.push_back(g); break

        Finder(vector<string> ofPower) {
            // convert
            for (string &line:ofPower) {
                WordInfo newWord;
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
            // format by length
            std::sort(words.begin(), words.end(), [](const WordInfo &a, const WordInfo &b) -> bool {
                return a.wordOfPower.size() >= b.wordOfPower.size();
            });
        };
#undef allCases

        const void print() {
            for (const WordInfo &wi: words) {
                glbLog() << "Word: '" << wi.wordOfPower << "', preprocessed: '" << wi.prepared << "', Moves: [";
                for (Actions a:wi.actions) {
                    glbLog() << ToString(a);
                }
                glbLog() << "]" << endl;
            }
        }

        const int FormatSolution(const Solution &moves, long id, long seed, Json::Value &root) {
            int bonus;
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
            for (const WordInfo &wi: words) {
                string::size_type idx = 0;
                int numRep = 0;
                while ((idx = sLine.find(wi.prepared, idx)) != string::npos) {
                    ++numRep;
                    sLine.replace(idx, wi.prepared.size(), wi.wordOfPower);
                    idx += wi.prepared.size();
                }
                if (numRep) {
                    bonus += 300 + numRep * 2 * wi.wordOfPower.size();
                }
            }

            Json::Value sub(Json::objectValue);
            sub["problemId"] = Json::Value::Int(id);
            sub["seed"] = Json::Value::Int(seed);
            sub["tag"] = string("B@OBab v1.6") + ((words.size() == 0) ? "f" : "w");
            sub["solution"] = sLine;
            root.append(sub);
            return bonus;
        }

        const vector<WordInfo> &Words() {
            return words;
        }
    };

}
#endif //ICFP2015_FINDER_H
