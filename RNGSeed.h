//
// Created by botanic on 8/7/15.
//

#ifndef ICFP2015_RNGSEED_H
#define ICFP2015_RNGSEED_H

#include <vector>
#include <json/value.h>

namespace icfp2015 {
    using std::vector;

    class RNG {
        const long Mul = 1103515245;
        const long Add = 12345;
        const long Mod = 0xFFFFFFFF; // 2^32
        long _rngVal;
        long _seed;
    public:
        RNG(long seed) : _rngVal(seed), _seed(seed) { };

        RNG(const RNG &r) : _rngVal(r._rngVal) { };

        const long seed() { return _seed; }

        void reset() { _rngVal = _seed; }

        long operator()() {
            long nextval = (_rngVal >> 16) & 0x7FFF;
            _rngVal = (_rngVal * Mul + Add) & Mod;
            return nextval;
        }
    };

    class RNGSeeds {
    public:
        vector<RNG> list;

        RNGSeeds(const Json::Value &root) {
            for (const Json::Value &nsd:root["sourceSeeds"]) {
                list.push_back(RNG(nsd.asInt()));
            }
        };
    };
}


#endif //ICFP2015_RNGSEED_H
