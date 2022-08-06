#pragma once

#include <cstdint>
#include <string_view>

using Hash_t = std::uint64_t;

namespace Hash
{
	static constexpr Hash_t FNV1A_BASIS = 0xCBF29CE484222325;
	static constexpr Hash_t FNV1A_PRIME = 0x100000001B3;

	Hash_t FNV1A( std::string_view wszText );
	Hash_t FNV1A( std::wstring_view wszText );

	consteval Hash_t ConstFNV1A( const char* pszText, const Hash_t ullValue = FNV1A_BASIS )
	{
		return ( pszText[ 0 ] == '\0' ) ? ullValue : ConstFNV1A( &pszText[ 1 ], ( ullValue ^ static_cast< Hash_t >( pszText[ 0 ] ) ) * FNV1A_PRIME );
	}

	consteval Hash_t ConstFNV1A( const wchar_t* wcsText, const Hash_t ullValue = FNV1A_BASIS )
	{
		return ( wcsText[ 0 ] == '\0' ) ? ullValue : ConstFNV1A( &wcsText[ 1 ], ( ullValue ^ static_cast< Hash_t >( wcsText[ 0 ] ) ) * FNV1A_PRIME );
	}
}

consteval Hash_t operator""_fnv1a( const char* pszText, std::size_t nLength )
{
	return Hash::ConstFNV1A( pszText );
}

consteval Hash_t operator""_fnv1a( const wchar_t* pszText, std::size_t nLength )
{
	return Hash::ConstFNV1A( pszText );
}