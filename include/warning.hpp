#pragma once

#include <map>

#include "log.hpp"


namespace logpp
{
	class Warning
	{
		public:
			static const std::string OUTPUT_WARNING_LOG_CALLBACK_NAME;

			enum class Type
			{
				DEFAULT,
				SUPPORT,
				SET,
				GET,
				CREATION,
				INIT,
				FILE
			};

			struct Data
			{
				logpp::Warning::Type type;
				bool isSuccessfull;
				logpp::Log log;
			};


			Warning() = delete;
			Warning(const logpp::Warning &) = delete;
			const Warning &operator=(const logpp::Warning &) = delete;


			static void init();

			static void addType(logpp::Warning::Type type, std::string_view name);

			static void subscribe(std::string_view name, const std::function<void(logpp::Warning::Data)> &callback);
			static void unsubscribe(std::string_view name);

			static void warn(logpp::Warning::Type type, std::string_view msg, const std::source_location &location = std::source_location::current());
			static void success(logpp::Warning::Type type, const std::source_location &location = std::source_location::current());

			static void synchronise();

		private:
			static std::map<logpp::Warning::Type, std::string> _types;
			static std::map<std::string, std::function<void(logpp::Warning::Data)>> _callbacks;
			static std::queue<std::future<void>> _callbackRet;
	};
}