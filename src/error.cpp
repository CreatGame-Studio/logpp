#include "error.hpp"



namespace logpp
{
	std::map<logpp::Error::Type, std::string> Error::_types {};



	void Error::init()
	{
		_types[logpp::Error::Type::CREATION] = "CREATION";
		_types[logpp::Error::Type::INIT] = "INIT";
		_types[logpp::Error::Type::DEFAULT] = "DEFAULT";
		_types[logpp::Error::Type::GET] = "GET";
		_types[logpp::Error::Type::SET] = "SET";
		_types[logpp::Error::Type::SUPPORT] = "SUPPORT";
	}
	


	void Error::addType(logpp::Error::Type type, std::string_view name)
	{
		_types[type] = name;
	}






	Error::Error(logpp::Error::Type type, std::string_view msg, const std::source_location &location) : _msg {}
	{
		logpp::Log log {logpp::Severity::ERROR, std::string(msg), location};
		log.msg = _types[type] + " | " + log.msg;

		_msg = logpp::Logger::getStringFromLog(log);
	}



	const std::string &Error::getMessage() const noexcept
	{
		return _msg;
	}



	const char *Error::what() const noexcept
	{
		return this->getMessage().c_str();
	}



}