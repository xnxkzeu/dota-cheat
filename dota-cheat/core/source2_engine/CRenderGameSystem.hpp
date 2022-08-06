#pragma once

#include "../helpers/memory.hpp"

#include "../types/matrix_t.hpp"

class CRenderGameSystem
{
public:
	VFUNC( 47, WorldToProjectionMatrix( ), ( this, 0 ), Math::ViewMatrix_t& );
};