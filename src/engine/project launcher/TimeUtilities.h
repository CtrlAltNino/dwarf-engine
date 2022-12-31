#pragma once

#include "../../utilities/dpch.h"

#define SECONDS_IN_MINUTE (60)
#define SECONDS_IN_HOUR (3600)
#define SECONDS_IN_DAY (86400)
#define SECONDS_IN_WEEK (604800)
#define SECONDS_IN_MONTH (2419200)
#define SECONDS_IN_YEAR (29030400)

namespace DPL
{
    /// @brief Class providing some time utilities.
    class TimeUtilities{
        public:
            /// @brief Gets the local time.
            /// @param time Pointer to a time stamp.
            /// @param buf Pointer to a date struct.
            static void GetLocalTime(time_t *time, struct tm *buf){
                #if _WIN32
                    localtime_s(buf, time);
                #elif __linux__
                    localtime_r(time, buf);
                #endif
            }

            /// @brief Calculates the time passed from a given time stamp, and formats it to a display string.
            /// @param passedTime A time stamp.
            /// @return The time passed formatted as a display string.
            static std::string CalculateTimePassedSinceNow(time_t passedTime){
                std::string timePassed = "";
                time_t currentTime = time(0);
                time_t timeDifference = currentTime - passedTime;

                if(timeDifference < SECONDS_IN_MINUTE){
                    timePassed = "a few seconds ago";
                }else if(timeDifference < SECONDS_IN_MINUTE * 2){
                    timePassed = "a minute ago";
                }else if(timeDifference < SECONDS_IN_HOUR){
                    timePassed = std::to_string((long long)floor(timeDifference / SECONDS_IN_MINUTE)) + " minutes ago";
                }else if(timeDifference < SECONDS_IN_HOUR * 2){
                    timePassed = "an hour ago";
                }else if(timeDifference < SECONDS_IN_DAY){
                    timePassed = std::to_string((long long)floor(timeDifference / (SECONDS_IN_HOUR))) + " hours ago";
                }else if(timeDifference < SECONDS_IN_DAY * 2){
                    timePassed = "a day ago";
                }else if(timeDifference < SECONDS_IN_WEEK){
                    timePassed = std::to_string((long long)floor(timeDifference / (SECONDS_IN_DAY))) + " days ago";
                }else if(timeDifference < SECONDS_IN_WEEK * 2){
                    timePassed = "a week ago";
                }else if(timeDifference < SECONDS_IN_MONTH){
                    timePassed = std::to_string((long long)floor(timeDifference / (SECONDS_IN_WEEK))) + " weeks ago";
                }else if(timeDifference < SECONDS_IN_MONTH * 2){
                    timePassed = "a month ago";
                }else if(timeDifference < SECONDS_IN_YEAR){
                    timePassed = std::to_string((long long)floor(timeDifference / (SECONDS_IN_MONTH))) + " months ago";
                }else if(timeDifference < SECONDS_IN_YEAR * 2){
                    timePassed = "a year ago";
                }else{
                    timePassed = std::to_string((long long)floor(timeDifference / (SECONDS_IN_YEAR))) + " years ago";
                }
                
                return timePassed;
            }
    };
}