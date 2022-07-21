#include "pch.h"
#include "EventsManager.hpp"
#include <chrono>

long double getTimeSinceLastEvent() 
{
	using highResClock = std::chrono::high_resolution_clock;

	static std::chrono::time_point<highResClock> mLastEvent;
	static bool mIsFirstEvent{ true };

	if (mIsFirstEvent)
	{
		mLastEvent = highResClock::now();
		mIsFirstEvent = false;

		return 0;
	}
	else
	{
		auto now{ highResClock::now() };
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - mLastEvent);
		long double ticksPassed{ static_cast<long double>(duration.count()) * 20 };
		mLastEvent = highResClock::now();

		return ticksPassed;
	}
}