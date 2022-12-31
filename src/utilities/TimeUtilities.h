#pragma once
#include "dpch.h"
#include<SDL2/SDL.h>

struct TimeStamp {
    Uint64 counter;
};

class TimeUtilities {
    public:
        static TimeStamp GetCurrent(){
            return { SDL_GetPerformanceCounter() };
        }

        static double GetDifferenceInSeconds(TimeStamp t1, TimeStamp t2){
            return (double)((t1.counter - t2.counter) / (double)SDL_GetPerformanceFrequency());
        }
};