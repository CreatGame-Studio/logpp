#include <iostream>
#include <sstream>

#include "log.hpp"


namespace logpp
{
	std::ostream *Logger::_stream {nullptr};
	std::mutex Logger::_streamMutex {};
	std::queue<std::future<void>> Logger::_asyncRet {};
	logpp::Severity Logger::_minSeverity {logpp::Severity::VERBOSE};



	void Logger::setStream(std::ostream *stream)
	{
		_stream = stream;
	}



	void Logger::setMinimalSeverity(logpp::Severity severity)
	{
		_minSeverity = severity;
	}



	void Logger::log(const logpp::Log &logData)
	{
		_asyncRet.push(std::async(logpp::Logger::_logHandler, logData));
	}



	void Logger::log(std::string_view msg)
	{
		_asyncRet.push(std::async(logpp::Logger::_logHandlerString, msg));
	}



	void Logger::verbose(std::string_view msg, const std::source_location &location)
	{
		logpp::Logger::log({logpp::Severity::VERBOSE, std::string(msg), location});
	}



	void Logger::performance(std::string_view msg, const std::source_location &location)
	{
		logpp::Logger::log({logpp::Severity::PERFORMANCE, std::string(msg), location});
	}



	void Logger::warning(std::string_view msg, const std::source_location &location)
	{
		logpp::Logger::log({logpp::Severity::WARNING, std::string(msg), location});
	}



	void Logger::error(std::string_view msg, const std::source_location &location)
	{
		logpp::Logger::log({logpp::Severity::ERROR, std::string(msg), location});
	}



	void Logger::synchronise()
	{
		while (!_asyncRet.empty())
		{
			_asyncRet.front().wait();
			_asyncRet.pop();
		}
	}



	std::string Logger::getStringFromLog(const logpp::Log &logData)
	{
		std::ostringstream stream {};
		
		std::string severity {};

		switch (logData.severity)
		{
			case logpp::Severity::VERBOSE:
				severity = "VERBOSE";
				break;

			case logpp::Severity::PERFORMANCE:
				severity = "PERFORMANCE";
				break;

			case logpp::Severity::WARNING:
				severity = "WARNING";
				break;

			case logpp::Severity::ERROR:
				severity = "ERROR";
				break;
		}

		stream << severity << " [thread = " << logData.thread << ", time = " << logData.time.count() << "] in file '" << logData.location.file_name() << "', function '" << logData.location.function_name() << "', line " << logData.location.line() << " : " << logData.msg;

		return stream.str();
	}



	void Logger::_logHandler(const logpp::Log &logData)
	{
		if (_stream == nullptr)
		{
			std::cerr << "log++ : you're trying to log to a null stream ! Please provide a valid stream with logpp::Logger::setStream(...)." << std::endl;
			return;
		}

		if (logData.severity < _minSeverity)
			return;

		std::string stringToOutput {logpp::Logger::getStringFromLog(logData)};

		_streamMutex.lock();
			*_stream << stringToOutput << std::endl;
		_streamMutex.unlock();
	}



	void Logger::_logHandlerString(std::string_view msg)
	{
		if (_stream == nullptr)
		{
			std::cerr << "log++ : you're trying to log to a null stream ! Please provide a valid stream with logpp::Logger::setStream(...)." << std::endl;
			return;
		}

		_streamMutex.lock();
			*_stream << msg << std::endl;
		_streamMutex.unlock();
	}
}