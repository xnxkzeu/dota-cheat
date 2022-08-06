#include <Windows.h>
#include <thread>

#include "core/helpers/logging.hpp"
#include "core/helpers/schema.hpp"
#include "core/helpers/input.hpp"
#include "core/helpers/render.hpp"

#include "hooks/hooks.hpp"

#include "menu.hpp"

#include "features/visuals.hpp"

#include "context/dota.hpp"
#include "context/mem.hpp"
#include "context/config.hpp"

void OnDllDetach( HMODULE hModule )
{
	Hooks::Detach( );

	pVisuals.reset( );

	Render::Detach( );

	Input::Detach( );

	Logging::Detach( );

	FreeLibrary( hModule );
}

void OnDllAttach( HMODULE hModule )
{
	DisableThreadLibraryCalls( hModule );

	try
	{
		Logging::Attach( );

		LOG( "Logging initialized." );

		CTX::DOTA = CTX::DOTA_t( );
		CTX::Memory = CTX::Memory_t( );
		CTX::Config = CTX::Config_t( );

		LOG( "Context initialized." );

		Schema::Setup( );

		LOG( "Schema initialized." );

		Input::Attach( CTX::DOTA->m_pD3D11Device );

		LOG( "Input initialized." );

		Render::Attach( CTX::DOTA->m_pD3D11Device, Input::GetWindowHandle( ) );

		LOG( "Render initialized." );

		Menu::Setup( );

		LOG( "Menu initialized." );

		pVisuals = std::make_unique< CVisuals >( );

		LOG( "Features initialized." );

		Hooks::Attach( );

		LOG( "Hook handler initialized." );

		Input::RegisterCallback( VK_END, std::bind_front( OnDllDetach, hModule ) );
	}
	catch( std::exception& exInfo )
	{
		LOG( "Something went wrong during initialization: {}", exInfo.what( ) );

		std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

		OnDllDetach( hModule );
	}
}

int DllMain( HMODULE hModule, DWORD dwReason, [[ maybe_unused ]] LPVOID lpReserved )
{
	if ( dwReason == DLL_PROCESS_ATTACH )
		std::thread( OnDllAttach, hModule ).detach( );

	return TRUE;
}