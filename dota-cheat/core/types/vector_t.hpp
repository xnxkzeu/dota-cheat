#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <cmath>

enum class Axis
	: std::size_t
{
	X = 0,
	Y,
	Z
};

enum class Rotation
	: std::size_t
{
	Pitch = 0,
	Yaw,
	Roll
};

namespace Math
{
	template< typename tContent, std::size_t nSize >
	struct Vector_t
	{
		Vector_t( ) = default;

		Vector_t( const std::array< tContent, nSize >& arrContent )
		{
			memcpy( m_arrContent.data( ), arrContent.data( ), sizeof( m_arrContent ) );
		}

		Vector_t( const Vector_t& vecOther )
		{
			memcpy( m_arrContent.data( ), vecOther.m_arrContent.data( ), sizeof( m_arrContent ) );
		}

		tContent& operator[]( std::size_t nIndex )
		{
			return this->m_arrContent[ nIndex ];
		}

		const tContent& operator[]( std::size_t nIndex ) const
		{
			return this->m_arrContent[ nIndex ];
		}

		tContent& operator[]( Axis nIndex )
		{
			return this->m_arrContent[ static_cast< std::size_t >( nIndex ) ];
		}

		const tContent& operator[]( Axis nIndex ) const
		{
			return this->m_arrContent[ static_cast< std::size_t >( nIndex ) ];
		}

		tContent& operator[]( Rotation nIndex )
		{
			return this->m_arrContent[ static_cast< std::size_t >( nIndex ) ];
		}

		const tContent& operator[]( Rotation nIndex ) const
		{
			return this->m_arrContent[ static_cast< std::size_t >( nIndex ) ];
		}

		template< std::size_t nVectorLength = nSize >
		const tContent GetDot( ) const
		{
			tContent valResult = { };

			for ( std::size_t nIndex = 0; nIndex < nVectorLength; ++nIndex )
				valResult += this->m_arrContent[ nIndex ] * this->m_arrContent[ nIndex ];

			return valResult;
		}

		template< std::size_t nVectorLength = nSize >
		const tContent GetLength( ) const
		{
			return std::sqrt( GetDot< nVectorLength >( ) );
		}

		template< std::size_t nVectorLength = nSize >
		const tContent Dot( const Vector_t& vecOther ) const
		{
			tContent valResult = { };

			for ( std::size_t nIndex = 0; nIndex < nVectorLength; ++nIndex )
				valResult += this->m_arrContent[ nIndex ] * vecOther.m_arrContent[ nIndex ];

			return valResult;
		}

		template< std::size_t nVectorLength = nSize >
		const tContent Length( const Vector_t& vecOther ) const
		{
			return std::sqrt( Dot< nVectorLength >( vecOther ) );
		}

		Vector_t operator+( const Vector_t& vecOther ) const
		{
			Vector_t vecValue = { };
			for ( std::size_t nIndex = 0; nIndex < nSize; nIndex++ )
				vecValue[ nIndex ] = this->m_arrContent[ nIndex ] + vecOther.m_arrContent[ nIndex ];

			return vecValue;
		}

		Vector_t operator-( const Vector_t& vecOther ) const
		{
			Vector_t vecValue = { };
			for ( std::size_t nIndex = 0; nIndex < nSize; nIndex++ )
				vecValue[ nIndex ] = this->m_arrContent[ nIndex ] - vecOther.m_arrContent[ nIndex ];

			return vecValue;
		}

		Vector_t operator*( const Vector_t& vecOther ) const
		{
			Vector_t vecValue = { };
			for ( std::size_t nIndex = 0; nIndex < nSize; nIndex++ )
				vecValue[ nIndex ] = this->m_arrContent[ nIndex ] * vecOther.m_arrContent[ nIndex ];

			return vecValue;
		}

		Vector_t operator/( const Vector_t& vecOther ) const
		{
			Vector_t vecValue = { };
			for ( std::size_t nIndex = 0; nIndex < nSize; nIndex++ )
				vecValue[ nIndex ] = this->m_arrContent[ nIndex ] / vecOther.m_arrContent[ nIndex ];

			return vecValue;
		}

	protected:
		std::array< tContent, nSize > m_arrContent = { };
	};
}