#pragma once

#include <stdexcept>
#include <format>
#include <minhook.h>

#include "../context/dota.hpp"
#include "../context/mem.hpp"

struct HookHandler_t;

namespace Hooks
{
	void Attach( );
	void Detach( );

	HookHandler_t& Get( );
}

inline void* HookFunction( void* pfnTarget, void* pfnDetour )
{
	void* pfnOriginal = nullptr;
	if ( const MH_STATUS nStatus = MH_CreateHook( pfnTarget, pfnDetour, &pfnOriginal ); nStatus != MH_OK )
		throw std::runtime_error( std::vformat( "Failed to hook function: {}.", std::make_format_args( MH_StatusToString( nStatus ) ) ) );

	return pfnOriginal;
}

inline void* HookFunction( void* pInstance, std::uint32_t nIndex, void* pfnDetour )
{
	void* pfnOriginal = nullptr;
	if ( const MH_STATUS nStatus = MH_CreateHook( ( *reinterpret_cast< void*** >( pInstance ) )[ nIndex ], pfnDetour, &pfnOriginal ); nStatus != MH_OK )
		throw std::runtime_error( std::vformat( "Failed to hook virtual function: {}.", std::make_format_args( MH_StatusToString( nStatus ) ) ) );

	return pfnOriginal;
}

#define HOOK_FUNCTION( pfnTarget, pfnDetour ) \
using fn ## pfnDetour ## _t = decltype( &pfnDetour ); \
fn ## pfnDetour ## _t m_pfn ## pfnDetour = reinterpret_cast< fn ## pfnDetour ## _t >( HookFunction( pfnTarget, reinterpret_cast< void* >( pfnDetour ) ) )

#define HOOK_VIRTUAL_FUNCTION( pInstance, nIndex, pfnDetour ) \
using fn ## pfnDetour ## _t = decltype( &pfnDetour ); \
fn ## pfnDetour ## _t m_pfn ## pfnDetour = reinterpret_cast< fn ## pfnDetour ## _t >( HookFunction( pInstance, nIndex, reinterpret_cast< void* >( pfnDetour ) ) )

struct IDXGISwapChain;

struct HookHandler_t
{
	HookHandler_t( ) = default;
	~HookHandler_t( ) = default;

	static void __fastcall OnRenderStart( CDOTAViewRender* pViewRender, void* pUnknown );

	static HRESULT __stdcall Present( IDXGISwapChain* pSwapchain, UINT unSyncInterval, UINT unFlags );
	static HRESULT __stdcall ResizeBuffers( IDXGISwapChain* pSwapchain, UINT unBufferCount, UINT unWidth, UINT unHeight, int nNewFormat, UINT unSwapchainFlags );

	HOOK_VIRTUAL_FUNCTION( CTX::DOTA->m_pViewRender, 4, OnRenderStart );

	HOOK_FUNCTION( CTX::Memory->IDXGISwapChain.pfnPresent, Present );
	HOOK_FUNCTION( CTX::Memory->IDXGISwapChain.pfnResizeBuffers, ResizeBuffers );
};