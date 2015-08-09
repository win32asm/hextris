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

    public:
        Solver(const icfp2015::RNG &rng, Field &f, const Units &u, const vector<WordInfo> &w) : field(f), units(u),
                                                                                                gen(rng), words(w) { }

        long Run(bool printField, int maxUnits) {
            field.reset();

            Simulate sim(field, units, gen, maxUnits);

            for (int i = 0; i < maxUnits; ++i) {

                if (!sim.nextUnit()) break;

                if (printField)
                    sim.print();

                bool hasSequences;
                Actions lastMove = Actions::MoveE;
                do {
                    hasSequences = false;
                    for (const WordInfo &wi:words) {
                        if (TrySequence(sim, wi.actions)) {
                            for (Actions a:wi.actions) {
                                sim.step(a);
                                if (a == Actions::MoveE || a == Actions::MoveW)
                                    lastMove = a;
                            }
                            hasSequences = true;
                            break; // one per unit?
                        }
                    }
                } while (hasSequences);

                if (printField)
                    sim.print();

                while (sim.step(lastMove, true) == VerifyState::Pass) {
                    sim.step(lastMove);
                }

                if (printField)
                    sim.print();

                // invert move
                lastMove = (lastMove == Actions::MoveE) ? Actions::MoveW : Actions::MoveE;

                while (true) {
                    if (sim.step(Actions::MoveSE, true) == VerifyState::Pass) {
                        sim.step(Actions::MoveSE);
                    } else if (sim.step(Actions::MoveSW, true) == VerifyState::Pass) {
                        sim.step(Actions::MoveSW);
                    } else {
                        break;
                    }
                    if (printField)
                        sim.print();
                }

                while (true) {
                    if (sim.step(lastMove, true) == VerifyState::Pass) {
                        sim.step(lastMove);
                    } else if (sim.step(Actions::MoveSE, true) == VerifyState::Pass) {
                        sim.step(Actions::MoveSE);
                    } else if (sim.step(Actions::MoveSW, true) == VerifyState::Pass) {
                        sim.step(Actions::MoveSW);
                    } else {
                        break;
                    }
                    if (printField)
                        sim.print();
                }

                // lock
                if (sim.step(Actions::MoveSW) != VerifyState::Lock &&
                    sim.step(Actions::MoveSE) != VerifyState::Lock) {
                    printf("\n\nHOLY CRAP!\n\n");
                }

                if (printField) {
                    sim.print();
                    printf("-------------- Step %i complete\n", i);
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
