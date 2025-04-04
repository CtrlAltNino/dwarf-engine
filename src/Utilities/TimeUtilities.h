#pragma once

#include <SDL3/SDL.h>
#include <fmt/format.h>

namespace Dwarf
{

#define SECONDS_IN_MINUTE (60)
#define SECONDS_IN_HOUR (3600)
#define SECONDS_IN_DAY (86400)
#define SECONDS_IN_WEEK (604800)
#define SECONDS_IN_MONTH (2419200)
#define SECONDS_IN_YEAR (29030400)

  struct TimeStamp
  {
    Uint64 counter;
  };

  class TimeUtilities
  {
  public:
    static auto
    GetCurrent() -> TimeStamp
    {
      return { SDL_GetPerformanceCounter() };
    }

    static auto
    GetDifferenceInSeconds(TimeStamp t1, TimeStamp t2) -> double
    {
      return (double)(t1.counter - t2.counter) /
             (double)SDL_GetPerformanceFrequency();
    }

    /// @brief Gets the local time.
    /// @param time Pointer to a time stamp.
    /// @param buf Pointer to a date struct.
    static void
    GetLocalTime(time_t const* time, struct tm* buf)
    {
#if _WIN32
      localtime_s(buf, time);
#elif __linux__
      localtime_r(time, buf);
#endif
    }

    /// @brief Calculates the time passed from a given time stamp, and formats
    /// it to a display string.
    /// @param passedTime A time stamp.
    /// @return The time passed formatted as a display string.
    static auto
    CalculateTimePassedSinceNow(time_t passedTime) -> std::string
    {
      std::string timePassed;
      time_t      currentTime = time(0);
      time_t      timeDifference = currentTime - passedTime;

      if (timeDifference < SECONDS_IN_MINUTE)
      {
        timePassed = "a few seconds ago";
      }
      else if (timeDifference < static_cast<time_t>(2) * SECONDS_IN_MINUTE)
      {
        timePassed = "a minute ago";
      }
      else if (timeDifference < SECONDS_IN_HOUR)
      {
        timePassed =
          fmt::format("{} minutes ago",
                      (long long)floor(timeDifference / SECONDS_IN_MINUTE));
      }
      else if (timeDifference < static_cast<time_t>(2) * SECONDS_IN_HOUR)
      {
        timePassed = "an hour ago";
      }
      else if (timeDifference < SECONDS_IN_DAY)
      {
        timePassed = fmt::format(
          "{} hours ago", (long long)floor(timeDifference / SECONDS_IN_HOUR));
      }
      else if (timeDifference < static_cast<time_t>(2) * SECONDS_IN_DAY)
      {
        timePassed = "a day ago";
      }
      else if (timeDifference < SECONDS_IN_WEEK)
      {
        timePassed = fmt::format(
          "{} days ago", (long long)floor(timeDifference / SECONDS_IN_DAY));
      }
      else if (timeDifference < static_cast<time_t>(2) * SECONDS_IN_WEEK)
      {
        timePassed = "a week ago";
      }
      else if (timeDifference < SECONDS_IN_MONTH)
      {
        timePassed = fmt::format(
          "{} weeks ago", (long long)floor(timeDifference / SECONDS_IN_WEEK));
      }
      else if (timeDifference < static_cast<time_t>(2) * SECONDS_IN_MONTH)
      {
        timePassed = "a month ago";
      }
      else if (timeDifference < SECONDS_IN_YEAR)
      {
        timePassed = fmt::format(
          "{} months ago", (long long)floor(timeDifference / SECONDS_IN_MONTH));
      }
      else if (timeDifference < static_cast<time_t>(2) * SECONDS_IN_YEAR)
      {
        timePassed = "a year ago";
      }
      else
      {
        timePassed = fmt::format(
          "{} years ago", (long long)floor(timeDifference / SECONDS_IN_YEAR));
      }

      return timePassed;
    }
  };
}