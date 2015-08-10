//
// Created by botanic on 8/9/15.
//

#ifndef ICFP2015_LOGGER_H
#define ICFP2015_LOGGER_H

#include <fstream>

namespace icfp2015 {
    using std::ofstream;
    using std::endl;

    class Logger {
        ofstream of_debug;
    public:
        ofstream &operator()() { return of_debug; }

        void Setup(long id) {
            char name[64];
            snprintf(name, 64, "debug_%li.log", id);
            of_debug.open(name, std::_S_out | std::_S_trunc);
        }

        ~Logger() {
            if (of_debug.is_open())
                of_debug.close();
        }
    };

    extern Logger glbLog;
}


#endif //ICFP2015_LOGGER_H
