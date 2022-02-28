#pragma once

#include <time.h>

class TimeUtilities{
    public:
        static void GetLocalTime(time_t *time, struct tm *buf){
            #if _WIN32
                localtime_s(buf, time);
            #elif __linux__
                localtime_r(time, buf);
            #endif
        }
};