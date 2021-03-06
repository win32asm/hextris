//
// Created by botanic on 8/7/15.
//

#include "units.h"

namespace icfp2015 {

    Units::Units(const Json::Value &root) {
        const Json::Value &ulist = root["units"];
        for (const Json::Value &udata:ulist) {
            Unit nUnit(udata);
            list.push_back(nUnit);
        }
    }
}