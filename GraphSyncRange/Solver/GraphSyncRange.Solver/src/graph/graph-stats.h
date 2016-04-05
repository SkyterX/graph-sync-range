#pragma once

#define COLLECT_TIME_STATS

#include <chrono>
#include <cassert>

namespace graph
{
	namespace stats
	{
		using TimeMeasure = std::chrono::nanoseconds;
		using ClockType = std::chrono::system_clock;
	}
}

#ifdef COLLECT_TIME_STATS
#define StartTimer() graph::stats::ClockType::time_point __GraphStatsStartTimeVar = graph::stats::ClockType::now()
#define GetTime() std::chrono::duration_cast<graph::stats::TimeMeasure>(graph::stats::ClockType::now() - __GraphStatsStartTimeVar)
#define UpdateTimer(Timer) Timer += std::chrono::duration_cast<graph::stats::TimeMeasure>(graph::stats::ClockType::now() - __GraphStatsStartTimeVar)

#else

#define StartTimer() ((void)0)
#define GetTime() graph::stats::ClockType::duration()
#define UpdateTimer(Timer) ((void)0)

#endif
