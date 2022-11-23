#pragma once

#include <string>
#include "Formatting.hxx"

namespace Basic
{
	struct Message
	{
		std::string string;

		template<size_t N_Characters, typename ...T>
		constexpr Message(char const (&msg)[N_Characters], T&& ...args)
		{
			if constexpr (sizeof...(args) > 0)
				string = Basic::Formatting::Format(msg, args...);
		}

		template<size_t N_Characters>
		constexpr Message(char const (&msg)[N_Characters])
		{
			string = Basic::Formatting::Format(msg);
		}
	}; 
}
