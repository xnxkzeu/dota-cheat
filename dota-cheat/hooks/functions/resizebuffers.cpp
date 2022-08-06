#include "../hooks.hpp"

#include "../../core/helpers/render.hpp"

HRESULT HookHandler_t::ResizeBuffers( IDXGISwapChain* pSwapchain, UINT unBufferCount, UINT unWidth, UINT unHeight, int nNewFormat, UINT unSwapchainFlags )
{
	const HRESULT lResult = Hooks::Get( ).m_pfnResizeBuffers( pSwapchain, unBufferCount, unWidth, unHeight, nNewFormat, unSwapchainFlags );

	Render::ResizeBuffers( pSwapchain, CTX::DOTA->m_pD3D11Device );

	return lResult;
}