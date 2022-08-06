#pragma once

#include <algorithm>

#include "vector_t.hpp"

namespace Math
{
	struct QAngle_t
	{
		QAngle_t( float flPitch = 0.f, float flYaw = 0.f, float flRoll = 0.f )
		{
			m_vecAngles[ Rotation::Pitch ] = flPitch;
			m_vecAngles[ Rotation::Yaw ] = flYaw;
			m_vecAngles[ Rotation::Roll ] = flRoll;

			Normalize( );
			Clamp( );
		}

		QAngle_t( const float* arrAngles )
		{
			memcpy( &m_vecAngles[ 0 ], arrAngles, sizeof( m_vecAngles ) );
		}

		void Normalize( )
		{
			m_vecAngles[ Rotation::Pitch ] = std::isfinite( m_vecAngles[ Rotation::Pitch ] ) ? std::remainderf( m_vecAngles[ Rotation::Pitch ], 180.f ) : 0.f;
			m_vecAngles[ Rotation::Yaw ] = std::isfinite( m_vecAngles[ Rotation::Yaw ] ) ? std::remainderf( m_vecAngles[ Rotation::Yaw ], 360.f ) : 0.f;
			m_vecAngles[ Rotation::Roll ] = std::clamp( m_vecAngles[ Rotation::Roll ], -50.f, 50.f );
		}

		void Clamp( )
		{
			m_vecAngles[ Rotation::Pitch ] = std::clamp( m_vecAngles[ Rotation::Pitch ], -89.f, 89.f );
			m_vecAngles[ Rotation::Yaw ] = std::clamp( m_vecAngles[ Rotation::Yaw ], -180.f, 180.f );
			m_vecAngles[ Rotation::Roll ] = std::clamp( m_vecAngles[ Rotation::Roll ], -50.f, 50.f );
		}

		float& GetPitch( )
		{
			return m_vecAngles[ Rotation::Pitch ];
		}

		const float& GetPitch( ) const
		{
			return m_vecAngles[ Rotation::Pitch ];
		}

		float& GetYaw( )
		{
			return m_vecAngles[ Rotation::Yaw ];
		}

		const float& GetYaw( ) const
		{
			return m_vecAngles[ Rotation::Yaw ];
		}

		float& GetRoll( )
		{
			return m_vecAngles[ Rotation::Roll ];
		}

		const float& GetRoll( ) const
		{
			return m_vecAngles[ Rotation::Roll ];
		}

		Math::Vector_t< float, 3 > m_vecAngles = { };
	};
}