#pragma once

#include "../helpers/memory.hpp"

class CSchemaSystemTypeScope;

class ISchemaSystem
{
public:
	VFUNC( 13, FindTypeScopeForModule( const char* pszModuleName ), ( this, pszModuleName ), CSchemaSystemTypeScope* );
};