#pragma once

#include <exception>
#include <map>

#include "log.hpp"


namespace logpp
{
	class Error : public std::exception
	{
		public:
			enum class Type
			{
				DEFAULT,
				SUPPORT,
				SET,
				GET,
				CREATION,
				INIT
			};

			
			static void init();

			static void addType(logpp::Error::Type type, std::string_view name);

			Error() = delete;
			Error(const logpp::Error &) = delete;
			const Error &operator=(const logpp::Error &) = delete;

			Error(logpp::Error::Type type, std::string_view msg, const std::source_location &location = std::source_location::current());

			const std::string &getMessage() const noexcept;
			virtual const char *what() const noexcept;

		private:
			static std::map<logpp::Error::Type, std::string> _types;

			std::string _msg;
	};
}