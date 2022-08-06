#pragma once

#include <array>
#include <cstdint>

enum Color : int
{
	Red = 0,
	Green,
	Blue,
	Alpha
};

struct Color_t
{
	Color_t( ) = default;

	constexpr Color_t( std::uint8_t uRed, std::uint8_t uGreen, std::uint8_t uBlue, std::uint8_t uAlpha )
	{
		m_arrColors[ Color::Red ] = uRed;
		m_arrColors[ Color::Green ] = uGreen;
		m_arrColors[ Color::Blue ] = uBlue;
		m_arrColors[ Color::Alpha ] = uAlpha;
	}

	Color_t( std::uint32_t unColor )
	{
		*reinterpret_cast< std::uint32_t* >( m_arrColors.data( ) ) = unColor;
	}

	operator std::uint32_t& ( )
	{
		return *reinterpret_cast< std::uint32_t* >( m_arrColors.data( ) );
	}

	operator const std::uint32_t& ( ) const
	{
		return *reinterpret_cast< const std::uint32_t* >( m_arrColors.data( ) );
	}

	std::uint8_t& operator[]( std::size_t nIndex )
	{
		return m_arrColors[ nIndex ];
	}

	const std::uint8_t& operator[]( std::size_t nIndex ) const
	{
		return m_arrColors[ nIndex ];
	}
private:
	std::array< std::uint8_t, 4 > m_arrColors = { };
};