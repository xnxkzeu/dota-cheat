#include <unordered_map>
#include <stdexcept>
#include <limits>
#include <array>
#include <thread>
#include <imgui.h>

#include "input.hpp"

#include "../../menu.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace Input
{
	std::unordered_map< Hash_t, KeyBindInfo_t > mapKeyBinds = { };
	std::array< std::function< void( ) >, std::numeric_limits< KeyID_t >::max( ) > arrCallbacks = { };

	HWND hWindow = nullptr;
	WNDPROC pfnOldWndProc = nullptr;

	LRESULT WINAPI WndProc( HWND hWindow, UINT unMsg, WPARAM wParam, LPARAM lParam )
	{
		KeyID_t uKeyID = 0;
		KeyState uKeyState = KeyState::Up;

		switch ( unMsg )
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			{
				uKeyID = VK_LBUTTON;
				uKeyState = KeyState::Down;
				break;
			}
			case WM_LBUTTONUP:
			{
				uKeyID = VK_LBUTTON;
				uKeyState = KeyState::Up;
				break;
			}
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			{
				uKeyID = VK_RBUTTON;
				uKeyState = KeyState::Down;
				break;
			}
			case WM_RBUTTONUP:
			{
				uKeyID = VK_RBUTTON;
				uKeyState = KeyState::Up;
				break;
			}
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
			{
				uKeyID = VK_MBUTTON;
				uKeyState = KeyState::Down;
				break;
			}
			case WM_MBUTTONUP:
			{
				uKeyID = VK_MBUTTON;
				uKeyState = KeyState::Up;
				break;
			}
			case WM_XBUTTONDOWN:
			case WM_XBUTTONDBLCLK:
			{
				uKeyID = GET_XBUTTON_WPARAM( wParam );
				uKeyState = KeyState::Down;
				break;
			}
			case WM_XBUTTONUP:
			{
				uKeyID = GET_XBUTTON_WPARAM( wParam );
				uKeyState = KeyState::Up;
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				uKeyID = static_cast< KeyID_t >( wParam );
				uKeyState = KeyState::Down;
				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				uKeyID = static_cast< KeyID_t >( wParam );
				uKeyState = KeyState::Up;
				break;
			}
			default:
				break;
		}

		if ( uKeyID )
		{
			if ( std::function< void( ) > fnCallback = arrCallbacks[ uKeyID ]; fnCallback && uKeyState == KeyState::Down )
				std::thread( fnCallback ).detach( );

			for ( auto& keyBindInfo : mapKeyBinds )
			{
				if ( keyBindInfo.second.m_uKeyID != uKeyID || keyBindInfo.second.m_uMode == KeyBindMode::Always )
					continue;

				switch ( keyBindInfo.second.m_uMode )
				{
					case KeyBindMode::Toggle:
					{
						keyBindInfo.second.m_bActive = !keyBindInfo.second.m_bActive;
						break;
					}
					case KeyBindMode::HoldOn:
					{
						keyBindInfo.second.m_bActive = ( uKeyState == KeyState::Down );
						break;
					}
					case KeyBindMode::HoldOff:
					{
						keyBindInfo.second.m_bActive = ( uKeyState == KeyState::Up );
						break;
					}
					default:
						break;
				}
			}
		}

		if ( Menu::IsVisible( ) )
		{
			ImGui_ImplWin32_WndProcHandler( hWindow, unMsg, wParam, lParam );
			return true;
		}
		
		return CallWindowProc( pfnOldWndProc, hWindow, unMsg, wParam, lParam );
	}

	void Attach( ID3D11Device* pD3D11Device )
	{
		hWindow = FindWindow( TEXT( "SDL_app" ), TEXT( "Dota 2" ) );
		if ( !hWindow )
			throw std::runtime_error( "Failed to find Dota 2 window." );

		pfnOldWndProc = reinterpret_cast< WNDPROC >( SetWindowLongPtr( hWindow, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( WndProc ) ) );
		if ( !pfnOldWndProc )
			throw std::runtime_error( "Failed to hook WndProc." );
	}

	void Detach( )
	{
		if ( pfnOldWndProc )
		{
			SetWindowLongPtr( hWindow, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( pfnOldWndProc ) );

			pfnOldWndProc = nullptr;
		}
	}

	void RegisterKeybind( Hash_t unHashedName, KeyID_t uKeyID, KeyBindMode uMode )
	{
		KeyBindInfo_t& keyBindInfo = mapKeyBinds[ unHashedName ];
		keyBindInfo.m_uKeyID = uKeyID;
		keyBindInfo.m_uMode = uMode;
		keyBindInfo.m_bActive = keyBindInfo.m_uMode == KeyBindMode::Always;
	}

	void RemoveKeybind( Hash_t unHashedName )
	{
		mapKeyBinds.erase( unHashedName );
	}

	void RegisterCallback( KeyID_t uKeyID, const std::function< void( ) >& fnCallback )
	{
		arrCallbacks[ uKeyID ] = fnCallback;
	}

	void RemoveCallback( KeyID_t uKeyID )
	{
		arrCallbacks[ uKeyID ] = nullptr;
	}

	HWND GetWindowHandle( )
	{
		return hWindow;
	}
}