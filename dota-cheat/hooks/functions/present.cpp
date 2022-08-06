#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "../hooks.hpp"

#include "../../core/helpers/render.hpp"

#include "../../menu.hpp"

HRESULT HookHandler_t::Present( IDXGISwapChain* pSwapchain, UINT unSyncInterval, UINT unFlags )
{
	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	Render::RenderDrawData( pSwapchain, CTX::DOTA->m_pD3D11Device, ImGui::GetBackgroundDrawList( ) );

	Menu::Render( );

	ImGui::Render( );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	return Hooks::Get( ).m_pfnPresent( pSwapchain, unSyncInterval, unFlags );
}