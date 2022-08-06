#pragma once

#include <string_view>
#include <format>

namespace Logging
{
	void Attach( );
	void Detach( );

	void Print( std::string_view szText );

	template< typename... tArgList >
	inline void Print( std::string_view szFormat, tArgList... argList )
	{
		Print( std::vformat( szFormat, std::make_format_args( argList... ) ) );
	}
}

#define LOG( szText... ) Logging::Print( szText )