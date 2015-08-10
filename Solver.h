//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_SOLVER_H
#define ICFP2015_SOLVER_H

#include <json/json.h>
#include <vector>
#include <string>
#include "Actions.h"
#include "RNGSeed.h"
#include "units.h"
#include "Field.h"
#include "Finder.h"
#include "Simulate.h"

namespace icfp2015 {

    using std::vector;
    using std::string;

    // tetris solver
    class Solver {
        Field &field;
        const Units &units;
        RNG gen;
        const vector<WordInfo> words;

        Solution sol;

        bool TrySequence(const Simulate &s, const vector<Actions> &word) {
            Simulate copyS(s);

            for (Actions a:word) {
                if (copyS.step(a) != VerifyState::Pass) return false;
            }
            return true;
        }

        void Try_Words(Simulate &sim, int printField) {
            bool hasSequences;
            do {
                hasSequences = false;
                for (const WordInfo &wi:words) {
                    if (TrySequence(sim, wi.actions)) {
                        for (Actions a:wi.actions) {
                            sim.step(a);
                            if (printField >= PRINT_ALL)
                                sim.print();
                        }
                        hasSequences = true;
                        if (printField >= PRINT_STEP)
                            sim.print(string("after word ") + wi.wordOfPower);
                    }
                }
            } while (hasSequences);
        }

        void Try_Shift(Simulate &sim, Actions dir, int printField) {
            while (sim.step(dir, true) == VerifyState::Pass) {
                sim.step(dir);
                if (printField >= PRINT_ALL)
                    sim.print();
            }

            if (printField >= PRINT_STEP)
                sim.print("After shift");
        }

        void Try_Shift_Words(Simulate &sim, int printField, bool allowDrop = false) {
            while (true) {
                Try_Words(sim, printField);

                if (sim.step(Actions::MoveE, true) == VerifyState::Pass) {
                    sim.step(Actions::MoveE);
                } else if (sim.step(Actions::MoveW, true) == VerifyState::Pass) {
                    sim.step(Actions::MoveW);
                } else if (allowDrop && sim.step(Actions::MoveSE, true) == VerifyState::Pass) {
                    sim.step(Actions::MoveSE);
                } else if (allowDrop && sim.step(Actions::MoveSW, true) == VerifyState::Pass) {
                    sim.step(Actions::MoveSW);
                } else {
                    break;
                }

                if (printField >= PRINT_ALL)
                    sim.print();
            }

            if (printField >= PRINT_STEP)
                sim.print("After shift+words");
        }

        void Try_Drop(Simulate &sim, int printField, Actions side = Actions::MoveE, int height = -1) {

            Actions down[3] = {Actions::MoveSE, Actions::MoveSW, Actions::MoveSE}; // hack
            int downIdx = (side == Actions::MoveE) ? 0 : 1;
            while ((height--) != 0) {
                if (sim.step(down[downIdx], true) == VerifyState::Pass) {
                    sim.step(down[downIdx]);
                } else if (sim.step(down[downIdx + 1], true) == VerifyState::Pass) {
                    sim.step(down[downIdx + 1]);
                } else {
                    break;
                }
                if (printField >= PRINT_ALL)
                    sim.print();
            }

            if (printField >= PRINT_STEP)
                sim.print("After drop");
        }

        Actions Try_Lock(Simulate &sim) {
            if (sim.step(Actions::MoveE, true) == VerifyState::Lock) {
                return Actions::MoveE;
            } else if (sim.step(Actions::MoveW, true) == VerifyState::Lock) {
                return Actions::MoveW;
            } else if (sim.step(Actions::MoveSE, true) == VerifyState::Lock) {
                return Actions::MoveSE;
            } else if (sim.step(Actions::MoveSW, true) == VerifyState::Lock) {
                return Actions::MoveSW;
            } else if (sim.step(Actions::TurnCCW, true) == VerifyState::Lock) {
                return Actions::TurnCCW;
            } else if (sim.step(Actions::TurnCW, true) == VerifyState::Lock) {
                return Actions::TurnCW;
            } else {
                return Actions::NoAction;
            }
        }

        void Try_Settle_more(Simulate &sim, int printField) {
            int turns = 0;
            vector<Actions> bestSol;
            vector<long> penalties;
            long leastPenalty = (Try_Lock(sim) == Actions::NoAction) ? INT64_MAX : sim.penalty();
            int round = 0;

            Actions dir = (sim.step(Actions::TurnCW, true) == VerifyState::Pass) ? Actions::TurnCW : Actions::TurnCCW;

            while (true) {
                Simulate newsim(sim);
                vector<Actions> newList;
                vector<Actions> canLockWith;
                vector<long> penalty;

                if (sim.penalty() != newsim.penalty()) {
                    glbLog() << "Worrysome!" << endl;
                }

                bool failed = false;
                for (int i = 0; !failed && i < turns; ++i) {
                    failed = newsim.step(dir) != VerifyState::Pass;
                    newList.push_back(dir);
                    penalty.push_back(newsim.penalty());
                    canLockWith.push_back(Try_Lock(newsim));
                }
                if (failed) // no more turns possible;
                    break;

                if (printField >= PRINT_DEBUG)
                    glbLog() << "trying " << turns << " turns round " << round << ": ";

                Actions tryOrder[4];

                if (round == 0) {
                    tryOrder[0] = Actions::MoveE;
                    tryOrder[1] = Actions::MoveSE;
                    tryOrder[2] = Actions::MoveSW;
                    tryOrder[3] = Actions::MoveW;
                } else if (round == 1) {
                    tryOrder[0] = Actions::MoveW;
                    tryOrder[1] = Actions::MoveSW;
                    tryOrder[2] = Actions::MoveSE;
                    tryOrder[3] = Actions::MoveE;
                } else if (round == 2) {
                    tryOrder[0] = Actions::MoveSW;
                    tryOrder[1] = Actions::MoveSE;
                    tryOrder[2] = Actions::MoveE;
                    tryOrder[3] = Actions::MoveW;
                }

                bool anyStep = true;
                while (anyStep) {
                    anyStep = false;
                    for (int i = 0; i < 4; ++i) {
                        if ((newsim.step(tryOrder[i], true)) == VerifyState::Pass) {
                            newsim.step(tryOrder[i]);
                            newList.push_back(tryOrder[i]);
                            penalty.push_back(newsim.penalty());
                            canLockWith.push_back(Try_Lock(newsim));
                            anyStep = true;
                            if (printField >= PRINT_DEBUG)
                                glbLog() << ToString(tryOrder[i]);
                            break;
                        }
                    }
                }

                if (penalty.size() != 0) {

                    while (true) {
                        auto score = min_element(penalty.begin(), penalty.end());
                        if (*score >= leastPenalty) {
                            if (printField >= PRINT_DEBUG)
                                glbLog() << " rejecting with best penalty " << *score;
                            break;
                        }
                        long index = distance(penalty.begin(), score);
                        if (canLockWith[index] == Actions::NoAction) {
                            *score = INT64_MAX;
                            if (printField >= PRINT_DEBUG)
                                glbLog() << " can not lock at penalty " << *score;
                            continue;
                        }
                        if (printField >= PRINT_DEBUG)
                            glbLog() << " using " << (index + 1) << " steps, penalty " << *score;
                        leastPenalty = *score;
                        bestSol = vector<Actions>(newList.begin(), newList.begin() + index + 1);
                        penalties = vector<long>(penalty.begin(), penalty.begin() + index + 1);
                        break;
                    }
                }

                if (++round == 3) {
                    ++turns;
                    round = 0;
                }
                if (printField >= PRINT_DEBUG)
                    glbLog() << endl;
            }
            for (Actions a:bestSol) {
                sim.step(a);
                if (printField >= PRINT_ALL)
                    sim.print();
            }

            if (bestSol.size() != 0 && sim.penalty() != leastPenalty) {
                glbLog() << "\n\n WTF again? \n\n" << endl;
            }

            if (printField >= PRINT_STEP)
                sim.print("After Settle more");
        }

        void Try_Settle(Simulate &sim, int printField, Actions side = Actions::MoveE) {
            Actions down[3] = {Actions::MoveSE, Actions::MoveSW, Actions::MoveSE}; // hack
            int downIdx = (side == Actions::MoveE) ? 0 : 1;
            Actions otherSide = (side == Actions::MoveE) ? Actions::MoveW : Actions::MoveE;
            while (true) {
                VerifyState thisSideFail;
                if ((thisSideFail = sim.step(side, true)) == VerifyState::Pass) {
                    sim.step(side);
                } else if (sim.step(down[downIdx], true) == VerifyState::Pass) {
                    sim.step(down[downIdx]);
                } else if (sim.step(down[downIdx + 1], true) == VerifyState::Pass) {
                    sim.step(down[downIdx + 1]);
                } else if (sim.step(Actions::TurnCW, true) == VerifyState::Pass) {
                    sim.step(Actions::TurnCW);
                } else if ((thisSideFail == VerifyState::Fail) && (sim.step(otherSide, true) == VerifyState::Pass)) {
                    sim.step(otherSide);
                } else {
                    break;
                }
                if (printField >= PRINT_ALL)
                    sim.print();
            }

            if (printField >= PRINT_STEP)
                sim.print("After Settle");
        }

    public:

        Solver(const icfp2015::RNG &rng, Field &f, const Units &u, const vector<WordInfo> &w) : field(f), units(u),
                                                                                                gen(rng), words(w) { }

        long Run(int printField, int maxUnits, int strategy = 2) {
            field.reset();

            Simulate sim(field, units, gen, maxUnits);

            for (int i = 0; i < maxUnits; ++i) {

                if (!sim.nextUnit()) break;

                if (printField >= PRINT_START)
                    sim.print("At start");

                if (strategy == 0) {

                    Try_Words(sim, printField);

                    Actions side = (sim.step(Actions::MoveE, true) == VerifyState::Pass) ? Actions::MoveE
                                                                                         : Actions::MoveW;

                    Try_Shift(sim, side, printField);

                    Try_Drop(sim, printField, side);

                } else if (strategy == 1) {
                    Try_Shift_Words(sim, printField);
                } else if (strategy == 2) {
                    Try_Shift_Words(sim, printField, true);
                }

                Actions side = (sim.step(Actions::MoveE, true) == VerifyState::Pass) ? Actions::MoveE : Actions::MoveW;

                //Try_Settle(sim, printField, side);
                Try_Settle_more(sim, printField);

                // lock
                Actions lock = Try_Lock(sim);

                if (lock == Actions::NoAction) {
                    glbLog() << "\n\nHOLY CRAP!\n\n" << endl;
                }

                sim.step(lock);

                if (printField >= PRINT_START) {
                    sim.print("At end");
                    glbLog() << "-------------- Step " << i << " complete" << endl;
                }
            }

            sol = sim.Moves();
            return sim.score();
        }

        const Solution getSol() const {
            return sol;
        }
    };
}

#endif //ICFP2015_SOLVER_H
