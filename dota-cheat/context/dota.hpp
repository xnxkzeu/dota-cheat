#pragma once

#include <optional>

#include "../core/helpers/memory.hpp"
#include "../core/helpers/interfaces.hpp"

#define ADD_INTERFACE( tInterface, pVariable, szModuleName, szInterfaceName ) tInterface* pVariable = Interfaces::Get( szModuleName, szInterfaceName ).Cast< tInterface* >( )
#define ADD_INTERFACE_CUSTOM( tInterface, pVariable, pAddress ) tInterface* pVariable = pAddress.Cast< tInterface* >( )
#define ADD_GAMESYSTEM( tGameSystem, ppVariable, szGameSystemName ) tGameSystem** ppVariable = Interfaces::GetGameSystem( szGameSystemName ).Cast< tGameSystem** >( )

class IVEngineClient2;
class ISchemaSystem;

class IMemAlloc;

struct ID3D11Device;
class CDOTAViewRender;
class CGameEntitySystem;

class CRenderGameSystem;

namespace CTX
{
	struct DOTA_t
	{
		ADD_INTERFACE( IVEngineClient2, m_pEngineClient, ENGINE2_DLL, "Source2EngineToClient" );
		ADD_INTERFACE( ISchemaSystem, m_pSchemaSystem, SCHEMASYSTEM_DLL, "SchemaSystem_" );
		
		ADD_INTERFACE_CUSTOM( ID3D11Device, m_pD3D11Device, Memory::FindPattern( RENDERSYSTEMDX11_DLL, "48 89 1D ? ? ? ? 48 89 3D" ).Jump( 0xA ).Get( ) );
		ADD_INTERFACE_CUSTOM( CDOTAViewRender, m_pViewRender, Memory::FindPattern( CLIENT_DLL, "48 8D 0D ? ? ? ? 8B D8 E8 ? ? ? ? 45 0F 57 ED" ).Jump( 0x3 ) );
		ADD_INTERFACE_CUSTOM( CGameEntitySystem, m_pEntitySystem, Memory::FindPattern( CLIENT_DLL, "48 8B 0D ? ? ? ? 48 85 C9 74 5B 48 8D 54 24" ).Jump( 0x3 ).Get( ) );

		ADD_GAMESYSTEM( CRenderGameSystem, m_ppRenderGameSystem, "RenderGameSystem" );
	};

	inline std::optional< DOTA_t > DOTA = std::nullopt; // @note xnxkzeu: std::optional is used here to delay initialization.
}

#undef ADD_INTERFACE
#undef ADD_INTERFACE_CUSTOM
#undef ADD_GAMESYSTEM