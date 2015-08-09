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
                        break; // one per unit?
                    }
                }
            } while (hasSequences);
            if (printField >= PRINT_STEP)
                sim.print();
        }

        void Try_Shift(Simulate &sim, Actions dir, int printField) {
            while (sim.step(dir, true) == VerifyState::Pass) {
                sim.step(dir);
                if (printField >= PRINT_ALL)
                    sim.print();
            }

            if (printField >= PRINT_STEP)
                sim.print();
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
                sim.print();
        }

        void Try_Settle(Simulate &sim, int printField, Actions side = Actions::MoveE) {
            Actions down[3] = {Actions::MoveSE, Actions::MoveSW, Actions::MoveSE}; // hack
            int downIdx = (side == Actions::MoveE) ? 0 : 1;
            while (true) {
                if (sim.step(side, true) == VerifyState::Pass) {
                    sim.step(side);
                } else if (sim.step(down[downIdx], true) == VerifyState::Pass) {
                    sim.step(down[downIdx]);
                } else if (sim.step(down[downIdx + 1], true) == VerifyState::Pass) {
                    sim.step(down[downIdx + 1]);
                } else if (sim.step(Actions::TurnCW, true) == VerifyState::Pass) {
                    sim.step(Actions::TurnCW);
                } else {
                    break;
                }
                if (printField >= PRINT_ALL)
                    sim.print();
            }
        }

    public:

        Solver(const icfp2015::RNG &rng, Field &f, const Units &u, const vector<WordInfo> &w) : field(f), units(u),
                                                                                                gen(rng), words(w) { }

        long Run(int printField, int maxUnits) {
            field.reset();

            Simulate sim(field, units, gen, maxUnits);

            for (int i = 0; i < maxUnits; ++i) {

                if (!sim.nextUnit()) break;

                if (printField >= PRINT_START)
                    sim.print();

                Try_Words(sim, printField);

                Actions side = (sim.step(Actions::MoveE, true) == VerifyState::Pass) ? Actions::MoveE : Actions::MoveW;

                Try_Shift(sim, side, printField);

                Try_Words(sim, printField);

                Try_Drop(sim, printField, side);

                // invert move
                side = (side == Actions::MoveE) ? Actions::MoveW : Actions::MoveE;

                Try_Settle(sim, printField, side);

                // lock
                if (sim.step(Actions::MoveSW) != VerifyState::Lock &&
                    sim.step(Actions::MoveSE) != VerifyState::Lock) {
                    printf("\n\nHOLY CRAP!\n\n");
                }

                if (printField >= PRINT_START) {
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
