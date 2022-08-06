#include <optional>

#include "hooks.hpp"

namespace Hooks
{
	std::optional< HookHandler_t > hookHandler = std::nullopt; // @note xnxkzeu: std::optional is used here only for delaying struct initialization

	void Attach( )
	{
		MH_Initialize( );

		hookHandler = HookHandler_t( );

		MH_EnableHook( MH_ALL_HOOKS );
	}

	void Detach( )
	{
		MH_Uninitialize( );

		hookHandler.reset( );
	}

	HookHandler_t& Get( )
	{
		return hookHandler.value( );
	}
}