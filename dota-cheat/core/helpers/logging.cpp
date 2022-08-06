#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <chrono>

#include "logging.hpp"

namespace Logging
{
	FILE* pStream = nullptr;
	std::ofstream ofsOutput = {};

	void Attach( )
	{
		ofsOutput.open( PROJECT_NAME ".log", std::ios::trunc | std::ios::binary );
		if ( !ofsOutput.is_open( ) )
			throw std::runtime_error( "Failed to open logging file for writing." );
		
#ifdef _DEBUG
		if ( !AllocConsole( ) )
			throw std::runtime_error( "Failed to allocate console." );

		AttachConsole( ATTACH_PARENT_PROCESS );

		if ( ( pStream = freopen( "CONOUT$", "w", stdout ) ); !pStream )
			throw std::runtime_error( "Failed to reopen stdout stream." );

		SetConsoleTitle( TEXT( PROJECT_NAME " debug console" ) );
#endif
	}

	void Detach( )
	{
		if ( ofsOutput.is_open( ) )
			ofsOutput.close( );

#ifdef _DEBUG
		if ( pStream )
			fclose( pStream );

		FreeConsole( );

		if ( const HWND hConsole = GetConsoleWindow( ) )
			PostMessage( hConsole, WM_CLOSE, 0, 0 );
#endif // _DEBUG
	}

	void Print( std::string_view szText )
	{
		const std::string szTime = std::format( "[{:%d-%m-%Y %X}] ", std::chrono::system_clock::now( ) );

#ifdef _DEBUG
		const HANDLE hOutput = GetStdHandle( STD_OUTPUT_HANDLE );

		SetConsoleTextAttribute( hOutput, FOREGROUND_RED | FOREGROUND_INTENSITY );
		std::cout << szTime;
		SetConsoleTextAttribute( hOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE );
		std::cout << szText << std::endl;
#endif // _DEBUG
		if ( ofsOutput.is_open( ) )
			ofsOutput << szTime << szText << std::endl;
	}
}