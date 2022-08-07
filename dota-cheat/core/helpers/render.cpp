#include <ShlObj.h>
#include <d3d11.h>
#include <filesystem>
#include <array>
#include <stdexcept>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <variant>
#include <string>
#include <atlbase.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui_internal.h>

#include <misc/freetype/imgui_freetype.h>

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "render.hpp"

#include "../source2_engine/CRenderGameSystem.hpp"

#include "../../context/dota.hpp"

namespace Render
{
	struct LineObject_t
	{
		ImVec2 m_vecStartPos = { };
		ImVec2 m_vecEndPos = { };
		ImU32 m_nColor = 0x00000000;
		float m_flThickness = 0.f;
	};

	struct RectObject_t
	{
		ImVec2 m_vecMin = { };
		ImVec2 m_vecMax = { };
		ImU32 m_nColor = 0x00000000;
		RectRenderFlags m_unFlags = { };
		ImU32 m_nOutlineColor = 0x00000000;
		float m_flRounding = 0.f;
		ImDrawFlags m_fRoundingCorners = ImDrawFlags_None;
		float m_flThickness = 0.f;
	};

	struct TextObject_t
	{
		ImVec2 m_vecPos = { };
		ImU32 m_nColor = 0x00000000;
		std::string m_szText = { };
	};

	using Object_t = std::variant< LineObject_t, RectObject_t, TextObject_t >;

	std::shared_mutex mutexRender = { };

	std::vector< Object_t > vecDrawData = { };
	std::vector< Object_t > vecSafeDrawData = { };

	Math::Vector_t< float, 2 > vecDisplaySize = { };

	ImGuiIO* pIO = nullptr;

	ID3D11RenderTargetView* pRenderTargetView = nullptr;

	void CreateRenderTarget( IDXGISwapChain* pSwapChain, ID3D11Device* pD3D11Device )
	{
		ID3D11Texture2D* pBackbuffer = nullptr;
		if ( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &pBackbuffer ) ) == S_OK )
		{
			D3D11_RENDER_TARGET_VIEW_DESC d3d11RenderTargetViewDesc = { };
			d3d11RenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			d3d11RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

			pD3D11Device->CreateRenderTargetView( pBackbuffer, &d3d11RenderTargetViewDesc, &pRenderTargetView );
			pBackbuffer->Release( );
		}
	}

	void Attach( ID3D11Device* pD3D11Device, HWND hWindow )
	{
		ID3D11DeviceContext* pDeviceContext = nullptr;
		pD3D11Device->GetImmediateContext( &pDeviceContext );
		if ( !pDeviceContext )
			throw std::runtime_error( "Failed to get d3d11 device context." );

		CComHeapPtr< wchar_t > pwszFontsPath = { };
		if ( FAILED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &pwszFontsPath ) ) )
			throw std::runtime_error( "Failed to get fonts directory path." );

		const std::filesystem::path pathFonts = pwszFontsPath.m_pData;

		ImVector< ImWchar > vecRanges = { };
		ImFontGlyphRangesBuilder rangesBuilder = { };

		static constexpr auto arrBaseRanges = std::to_array< ImWchar >( {
				0x0100, 0x024F,
				0x0300, 0x03FF,
				0x0600, 0x06FF,
				0x0E00, 0x0E7F,
				0
			}
		);

		ImGui::CreateContext( );

		pIO = &ImGui::GetIO( );
		pIO->IniFilename = nullptr;
		pIO->LogFilename = nullptr;

		rangesBuilder.AddRanges( arrBaseRanges.data( ) );
		rangesBuilder.AddRanges( pIO->Fonts->GetGlyphRangesCyrillic( ) );
		rangesBuilder.BuildRanges( &vecRanges );

		ImFontConfig verdanaConfig = {};
		verdanaConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
		pIO->Fonts->AddFontFromFileTTF( ( pathFonts / "Verdana.ttf" ).string( ).c_str( ), 14.f, &verdanaConfig, vecRanges.Data );
		pIO->Fonts->Build( );

		ImGui_ImplWin32_Init( hWindow );
		ImGui_ImplDX11_Init( pD3D11Device, pDeviceContext );
	}

	void Detach( )
	{
		if ( pRenderTargetView )
		{
			pRenderTargetView->Release( );
			pRenderTargetView = nullptr;
		}

		if ( ImGui::GetCurrentContext( ) )
		{
			ImGui_ImplDX11_Shutdown( );
			ImGui_ImplWin32_Shutdown( );

			ImGui::DestroyContext( );
		}
	}

	void RenderDrawData( IDXGISwapChain* pSwapChain, ID3D11Device* pD3D11Device, ImDrawList* pDrawList )
	{
		std::unique_lock< std::shared_mutex > lock( mutexRender );

		if ( !pRenderTargetView )
			CreateRenderTarget( pSwapChain, pD3D11Device );

		if ( pRenderTargetView )
		{
			ID3D11DeviceContext* pDeviceContext = nullptr;
			pD3D11Device->GetImmediateContext( &pDeviceContext );
			if ( pDeviceContext )
				pDeviceContext->OMSetRenderTargets( 1, &pRenderTargetView, nullptr );
		}
		
		struct ObjectVisitor_t
		{
			ObjectVisitor_t( ImDrawList* pDrawList = nullptr )
				: m_pDrawList( pDrawList )
			{
			}

			ImDrawList* m_pDrawList = nullptr;

			void operator()( const LineObject_t& lineObject )
			{
				m_pDrawList->AddLine( lineObject.m_vecStartPos, lineObject.m_vecEndPos, lineObject.m_nColor, lineObject.m_flThickness );
			}

			void operator()( const RectObject_t& rectObject )
			{
				if ( rectObject.m_unFlags & RectRenderFlags::Filled )
				{
					if ( rectObject.m_unFlags & RectRenderFlags::Outline )
						m_pDrawList->AddRectFilled( rectObject.m_vecMin - ImVec2( 1.f, 1.f ), rectObject.m_vecMax + ImVec2( 1.f, 1.f ), rectObject.m_nOutlineColor, rectObject.m_flRounding, rectObject.m_fRoundingCorners );

					m_pDrawList->AddRectFilled( rectObject.m_vecMin, rectObject.m_vecMax, rectObject.m_nColor, rectObject.m_flRounding, rectObject.m_fRoundingCorners );
				}
				else
				{
					if ( rectObject.m_unFlags & RectRenderFlags::Outline )
						m_pDrawList->AddRect( rectObject.m_vecMin - ImVec2( 1.f, 1.f ), rectObject.m_vecMax + ImVec2( 1.f, 1.f ), rectObject.m_nOutlineColor, rectObject.m_flRounding, rectObject.m_fRoundingCorners, rectObject.m_flThickness + 2.f );

					m_pDrawList->AddRect( rectObject.m_vecMin, rectObject.m_vecMax, rectObject.m_nColor, rectObject.m_flRounding, rectObject.m_fRoundingCorners, rectObject.m_flThickness );
				}
			}

			void operator()( const TextObject_t& textObject )
			{
				m_pDrawList->AddText( textObject.m_vecPos, textObject.m_nColor, textObject.m_szText.c_str( ) );
			}
		};

		ObjectVisitor_t objectVisitor( pDrawList );

		for ( const auto& objectInfo : vecSafeDrawData )
			std::visit( objectVisitor, objectInfo );
	}

	void ClearDrawData( )
	{
		std::lock_guard< std::shared_mutex > lock( mutexRender );

		if ( !vecDrawData.empty( ) )
			vecDrawData.clear( );

		vecDisplaySize = *reinterpret_cast< Math::Vector_t< float, 2 >* >( &pIO->DisplaySize );
	}

	void SwapDrawData( )
	{
		std::lock_guard< std::shared_mutex > lock( mutexRender );

		vecDrawData.swap( vecSafeDrawData );
	}

	void ResizeBuffers( IDXGISwapChain* pSwapChain, ID3D11Device* pD3D11Device )
	{
		if ( pRenderTargetView )
		{
			pRenderTargetView->Release( );
			pRenderTargetView = nullptr;
		}

		CreateRenderTarget( pSwapChain, pD3D11Device );
	}

	void Line( const Math::Vector_t< float, 2 >& vecStart, const Math::Vector_t< float, 2 >& vecEnd, const Color_t colLine, float flThickness )
	{
		vecDrawData.emplace_back(
			LineObject_t{
				*reinterpret_cast< const ImVec2* >( &vecStart ),
				*reinterpret_cast< const ImVec2* >( &vecEnd ),
				colLine,
				flThickness
			}
		);
	}

	void Rect( const Math::Vector_t< float, 2 >& vecMin, const Math::Vector_t< float, 2 >& vecMax, const Color_t colRect, std::uint32_t fFlags, const Color_t colOutline, float flRounding, ImDrawFlags fRoundingCorners, float flThickness )
	{
		vecDrawData.emplace_back(
			RectObject_t{
				*reinterpret_cast< const ImVec2* >( &vecMin ),
				*reinterpret_cast< const ImVec2* >( &vecMax ),
				colRect,
				static_cast< RectRenderFlags >( fFlags ),
				colOutline,
				flRounding,
				fRoundingCorners,
				flThickness
			}
		);
	}

	void Text( const Math::Vector_t< float, 2 >& vecPos, const char* pszText, const Color_t colText )
	{
		vecDrawData.emplace_back(
			TextObject_t{
				*reinterpret_cast< const ImVec2* >( &vecPos ),
				colText,
				pszText
			}
		);
	}

	bool WorldToScreen( const Math::Vector_t< float, 3 >& vecWorld, Math::Vector_t< float, 2 >& vecOutScreen )
	{
		CRenderGameSystem* pRenderGameSystem = *CTX::DOTA->m_ppRenderGameSystem;
		if ( !pRenderGameSystem )
			return false;

		const Math::ViewMatrix_t& matWorldToScreen = pRenderGameSystem->WorldToProjectionMatrix( );
		const float flWidth = matWorldToScreen[ 3 ][ 0 ] * vecWorld[ 0 ] + matWorldToScreen[ 3 ][ 1 ] * vecWorld[ 1 ] + matWorldToScreen[ 3 ][ 2 ] * vecWorld[ 2 ] + matWorldToScreen[ 3 ][ 3 ];

		if ( flWidth < 0.001f )
			return false;

		const float flInverse = 1.0f / flWidth;
		vecOutScreen[ 0 ] = ( matWorldToScreen[ 0 ][ 0 ] * vecWorld[ 0 ] + matWorldToScreen[ 0 ][ 1 ] * vecWorld[ 1 ] + matWorldToScreen[ 0 ][ 2 ] * vecWorld[ 2 ] + matWorldToScreen[ 0 ][ 3 ] ) * flInverse;
		vecOutScreen[ 1 ] = ( matWorldToScreen[ 1 ][ 0 ] * vecWorld[ 0 ] + matWorldToScreen[ 1 ][ 1 ] * vecWorld[ 1 ] + matWorldToScreen[ 1 ][ 2 ] * vecWorld[ 2 ] + matWorldToScreen[ 1 ][ 3 ] ) * flInverse;

		vecOutScreen[ 0 ] = std::floor( ( vecDisplaySize[ 0 ] * 0.5f ) + ( vecOutScreen[ 0 ] * vecDisplaySize[ 0 ] ) * 0.5f );
		vecOutScreen[ 1 ] = std::floor( ( vecDisplaySize[ 1 ] * 0.5f ) - ( vecOutScreen[ 1 ] * vecDisplaySize[ 1 ] ) * 0.5f );
		return true;
	}
}