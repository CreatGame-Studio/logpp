#pragma once

#include "error.hpp"
#include "warning.hpp"


namespace logpp
{
	void init();
	void quit();

	inline const std::tuple<int, int, int> version = {1, 0, 0};
}