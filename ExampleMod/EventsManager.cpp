#include "pch.h"
#include "EventsManager.hpp"
#include <chrono>

/**
 * \brief Gets the time elapsed since the last event in ticks
 * \return Time since last piston action in ticks
 */
long double getTimeSinceLastEvent() 
{
	using highResClock = std::chrono::high_resolution_clock;

	static std::chrono::time_point<highResClock> lastEvent;
	static bool isFirstEvent{ true };

	if (isFirstEvent)
	{
		lastEvent = highResClock::now();
		isFirstEvent = false;

		return 0;
	}
	else
	{
		const auto now{ highResClock::now() };
		const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastEvent);
		const long double ticksPassed{ static_cast<long double>(duration.count()) * 20 };
		lastEvent = highResClock::now();

		return ticksPassed;
	}
}