#pragma once

#include "../../types/vector_t.hpp"
#include "../../types/qangle_t.hpp"

#include "../../helpers/schema.hpp"

class CGameSceneNode
{
public:
	SCHEMA_VARIABLE( "CGameSceneNode", "m_vecAbsOrigin", GetAbsOrigin, Math::Vector_t< float, 3 > );
	SCHEMA_VARIABLE( "CGameSceneNode", "m_angAbsRotation", GetAbsRotation, Math::QAngle_t );
};