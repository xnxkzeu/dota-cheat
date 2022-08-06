#include <stdexcept>
#include <format>

#include "interfaces.hpp"

namespace Interfaces
{
	Memory::Address_t Get( std::string_view szModuleName, std::string_view szInterfaceName )
	{
		class CInterfaceRegister
		{
		public:
			void* ( __cdecl* m_pfnCreate )( );
			const char* m_pszName;
			CInterfaceRegister* m_pNext;
		};

		const auto GetRegisterList = [ &szModuleName ]( ) -> CInterfaceRegister*
		{
			Memory::Address_t pfnCreateInterface = Memory::GetExportAddress( Memory::GetModuleBaseAddress( szModuleName ), "CreateInterface" );
			if ( !pfnCreateInterface )
				throw std::runtime_error( "Failed to get CreateInterface address." );

			return pfnCreateInterface.Jump( 0x3 ).Get( ).Cast< CInterfaceRegister* >( );
		};

		for ( CInterfaceRegister* pRegister = GetRegisterList( ); pRegister != nullptr; pRegister = pRegister->m_pNext )
		{
			if ( ( std::string_view( pRegister->m_pszName ).compare( 0U, szInterfaceName.length( ), szInterfaceName ) == 0 &&
				std::atoi( pRegister->m_pszName + szInterfaceName.length( ) ) > 0 ) ||
				szInterfaceName.compare( pRegister->m_pszName ) == 0 )
			{
				return reinterpret_cast< std::uintptr_t >( pRegister->m_pfnCreate( ) );
			}
		}

		throw std::runtime_error( std::format( "Failed to find interface \"{}\" in module \"{}\".", szInterfaceName, szModuleName ) );
	}

	Memory::Address_t GetGameSystem( std::string_view szGameSystemName )
	{
		class IGameSystemFactory
		{
			[[ maybe_unused ]]
			std::byte m_pad0[ 0x8 ];
		public:
			IGameSystemFactory* m_pNextFactory;
			const char* m_pszName;
			void* m_pGameSystem;
		};

		IGameSystemFactory* pFactory = Memory::FindPattern( CLIENT_DLL, "4C 8B 35 ? ? ? ? 4D 85 F6 0F 84 ? ? ? ? 41 BF" ).Jump( 0x3 ).Get( ).Cast < IGameSystemFactory* >( );
		if ( !pFactory )
			throw std::runtime_error( "Failed to find game system factory list." );

		for ( ; pFactory; pFactory = pFactory->m_pNextFactory )
		{
			if ( szGameSystemName.compare( pFactory->m_pszName ) != 0 )
				continue;

			return pFactory->m_pGameSystem;
		}

		throw std::runtime_error( std::format( "Failed to find \"{}\" game system.", szGameSystemName ) );
	}
}