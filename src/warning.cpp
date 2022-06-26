#include <iostream>

#include "warning.hpp"


namespace logpp
{
	const std::string Warning::OUTPUT_WARNING_LOG_CALLBACK_NAME {"___DEFAULT_OUTPUT_WARNING_LOG_CALLBACK"};

	std::map<logpp::Warning::Type, std::string> Warning::_types {};
	std::map<std::string, std::function<void(logpp::Warning::Data)>> Warning::_callbacks {};
	std::queue<std::future<void>> Warning::_callbackRet {};




	void Warning::init()
	{
		_types[logpp::Warning::Type::DEFAULT] = "DEFAULT";
		_types[logpp::Warning::Type::SUPPORT] = "SUPPORT";
		_types[logpp::Warning::Type::SET] = "SET";
		_types[logpp::Warning::Type::GET] = "GET";
		_types[logpp::Warning::Type::CREATION] = "CREATION";
		_types[logpp::Warning::Type::INIT] = "INIT";
		_types[logpp::Warning::Type::FILE] = "FILE";

		_callbacks[logpp::Warning::OUTPUT_WARNING_LOG_CALLBACK_NAME] = [] (logpp::Warning::Data data)
		{
			if (!data.isSuccessfull)
			{
				logpp::Logger::log(data.log);
			}
		};
	}



	void Warning::addType(logpp::Warning::Type type, std::string_view name)
	{
		_types[type] = name;
	}



	void Warning::subscribe(std::string_view name, const std::function<void(logpp::Warning::Data)> &callback)
	{
		_callbacks[std::string(name)] = callback;
	}



	void Warning::unsubscribe(std::string_view name)
	{
		auto it {_callbacks.find(std::string(name))};
		if (it == _callbacks.end())
			return;

		_callbacks.erase(it);
	}



	void Warning::warn(logpp::Warning::Type type, std::string_view msg, const std::source_location &location)
	{
		logpp::Warning::Data data {type, false, {logpp::Severity::WARNING, "", location}};
		data.log.msg = _types[type] + " | " + std::string(msg);

		for (auto callback : _callbacks)
		{
			_callbackRet.push(std::async(callback.second, data));
		}
	}



	void Warning::success(logpp::Warning::Type type, const std::source_location &location)
	{
		logpp::Warning::Data data {type, true, {logpp::Severity::WARNING, "", location}};

		for (auto callback : _callbacks)
		{
			_callbackRet.push(std::async(callback.second, data));
		}
	}



	void Warning::synchronise()
	{
		while (!_callbackRet.empty())
		{
			_callbackRet.front().wait();
			_callbackRet.pop();
		}
	}



}