#include "logpp.hpp"


namespace logpp
{
	void init()
	{
		logpp::Error::init();
		logpp::Warning::init();
	}



	void quit()
	{
		logpp::Warning::synchronise();
		logpp::Logger::synchronise();
	}



}