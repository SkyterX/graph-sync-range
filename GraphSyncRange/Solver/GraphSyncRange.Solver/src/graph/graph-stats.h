#pragma once

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

#ifndef NO_TIME_STATS

#define UpdateTimestampVar(timestampVariable) timestampVariable = graph::stats::ClockType::now()
#define CreateTimestampVar(timestampVariable) graph::stats::ClockType::time_point UpdateTimestampVar(timestampVariable)
#define GetTimeByVar(timestampVariable) std::chrono::duration_cast<graph::stats::TimeMeasure>(graph::stats::ClockType::now() - timestampVariable)
#define UpdateTimerByVar(Timer, timestampVariable) Timer += GetTimeByVar(timestampVariable)

#else

#define UpdateTimestampVar(timestampVariable) ((void)0)
#define CreateTimestampVar(timestampVariable) ((void)0)
#define GetTimeByVar(timestampVariable) graph::stats::ClockType::duration()
#define UpdateTimerByVar(Timer, timestampVariable) ((void)0)

#endif

#define DefaultTimestamp __GraphStatsStartTimeVar
#define UpdateTimestamp() UpdateTimestampVar(DefaultTimestamp)
#define CreateTimestamp() CreateTimestampVar(DefaultTimestamp)
#define GetTime() GetTimeByVar(DefaultTimestamp)
#define UpdateTimer(Timer) UpdateTimerByVar(Timer, DefaultTimestamp)
