#pragma once

#include <array>
#include <cstddef>

namespace Math
{
	template< typename tContent, std::size_t nSizeX, std::size_t nSizeY >
	struct Matrix_t
	{
		Matrix_t( ) = default;

		constexpr std::array< tContent, nSizeY >& operator[]( std::size_t nIndex )
		{
			return this->m_arrContent[ nIndex ];
		}

		constexpr const std::array< tContent, nSizeY >& operator[]( std::size_t nIndex ) const
		{
			return this->m_arrContent[ nIndex ];
		}

		std::array< std::array< tContent, nSizeY >, nSizeX > m_arrContent = { };
	};

	using ViewMatrix_t = Matrix_t< float, 4, 4 >;
}