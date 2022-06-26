#pragma once

#include <future>
#include <queue>
#include <source_location>
#include <string>
#include <thread>

#include "time.hpp"


namespace logpp
{
	/**
	 * @brief An enum that represents differents log severities
	 * 
	 */
	enum class Severity
	{
		VERBOSE,
		PERFORMANCE,
		WARNING,
		ERROR
	};

	/**
	 * @brief This struct contains all information about a log
	 * 
	 */
	struct Log
	{
		logpp::Severity severity;
		std::string msg;
		std::source_location location {std::source_location::current()};
		std::thread::id thread {std::this_thread::get_id()};
		std::chrono::milliseconds time {logpp::getTimeSinceProgramStarted()}; // if you have your own system to get time since the program started, replace logpp::getTimeSinceProgramStarted() by your own implement
	};


	/**
	 * @brief This class is a collection of static methods which handle logging
	 * 
	 */
	class Logger
	{
		public:
			Logger() = delete;
			Logger(const Logger &) = delete;
			const Logger &operator=(const Logger &) = delete;


			static void setStream(std::ostream *stream);
			static void setMinimalSeverity(logpp::Severity severity);

			static void log(const logpp::Log &logData);
			static void log(std::string_view msg);
			static void verbose(std::string_view msg, const std::source_location &location = std::source_location::current());
			static void performance(std::string_view msg, const std::source_location &location = std::source_location::current());
			static void warning(std::string_view msg, const std::source_location &location = std::source_location::current());
			static void error(std::string_view msg, const std::source_location &location = std::source_location::current());

			static void synchronise();

			static std::string getStringFromLog(const logpp::Log &logData);

		private:
			static void _logHandler(const logpp::Log &logData);
			static void _logHandlerString(std::string_view msg);

			static std::ostream *_stream;
			static std::mutex _streamMutex;
			static std::queue<std::future<void>> _asyncRet;
			static logpp::Severity _minSeverity;
	};
};