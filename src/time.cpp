#include "time.hpp"


namespace logpp
{
	std::chrono::steady_clock::time_point _start {std::chrono::steady_clock::now()};

	std::chrono::milliseconds getTimeSinceProgramStarted()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - _start);
	}
}