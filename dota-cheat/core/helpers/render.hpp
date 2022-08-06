#pragma once

#include <Windows.h>

#include <imgui.h>

#include "../types/vector_t.hpp"
#include "../types/color_t.hpp"

enum RectRenderFlags
{
	Outline = ( 1 << 0 ),
	Filled = ( 1 << 1 )
};

struct ID3D11Device;
struct IDXGISwapChain;

namespace Render
{
	void Attach( ID3D11Device* pD3D11Device, HWND hWindow );
	void Detach( );

	void RenderDrawData( IDXGISwapChain* pSwapChain, ID3D11Device* pD3D11Device, ImDrawList* pDrawList );
	void ClearDrawData( );
	void SwapDrawData( );

	void ResizeBuffers( IDXGISwapChain* pSwapChain, ID3D11Device* pD3D11Device );

	void Line( const Math::Vector_t< float, 2 >& vecStart, const Math::Vector_t< float, 2 >& vecEnd, const Color_t colLine, float flThickness = 1.f );
	void Rect( const Math::Vector_t< float, 2 >& vecMin, const Math::Vector_t< float, 2 >& vecMax, const Color_t colRect, std::uint32_t fFlags = { }, const Color_t colOutline = Color_t( 0, 0, 0, 255 ), float flRounding = 0.f, ImDrawFlags fRoundingCorners = ImDrawFlags_RoundCornersAll, float flThickness = 1.f );
	void Text( const Math::Vector_t< float, 2 >& vecPos, const char* pszText, const Color_t colText );

	bool WorldToScreen( const Math::Vector_t< float, 3 >& vecWorld, Math::Vector_t< float, 2 >& vecOutScreen );
}