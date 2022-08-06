#include "../hooks.hpp"

#include "../../core/helpers/render.hpp"

#include "../../features/visuals.hpp"

void HookHandler_t::OnRenderStart( CDOTAViewRender* pViewRender, void* pUnknown )
{
	Render::ClearDrawData( );

	pVisuals->OnRender( );
	
	Render::SwapDrawData( );

	return Hooks::Get( ).m_pfnOnRenderStart( pViewRender, pUnknown );
}