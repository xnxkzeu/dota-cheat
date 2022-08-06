#pragma once

#include <cstdint>
#include <functional>
#include <Windows.h>
#include <d3d11.h>

#include "hash.hpp"

using KeyID_t = std::uint8_t;

namespace Input
{
	enum class KeyBindMode : std::uint8_t
	{
		Always = 0,
		Toggle,
		HoldOn,
		HoldOff
	};

	enum class KeyState : std::uint8_t
	{
		Up = 0,
		Down,
	};

	struct KeyBindInfo_t
	{
		KeyID_t m_uKeyID = 0;
		KeyBindMode m_uMode = KeyBindMode::Always;
		bool m_bActive = true;
	};

	void Attach( ID3D11Device* pD3D11Device );
	void Detach( );

	void RegisterKeybind( Hash_t unHashedName, KeyID_t uKeyID, KeyBindMode uMode );
	void RemoveKeybind( Hash_t unHashedName );

	void RegisterCallback( KeyID_t uKeyID, const std::function< void( ) >& fnCallback );
	void RemoveCallback( KeyID_t uKeyID );

	HWND GetWindowHandle( );
}