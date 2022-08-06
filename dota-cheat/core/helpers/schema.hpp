#pragma once

#include <cstdint>
#include <type_traits>

#include "hash.hpp"

namespace Schema
{
	void Setup( );

	std::uint32_t Get( Hash_t ullClassHashed, Hash_t ullFieldHashed );
}

#define SCHEMA_VARIABLE_OFFSET( szClassName, szFieldName, szFunctionName, nAdditional, tReturn... ) \
[[ nodiscard ]] std::add_lvalue_reference_t< tReturn > szFunctionName( ) \
{ \
	static std::uint32_t unOffset = Schema::Get( Hash::ConstFNV1A( szClassName ), Hash::ConstFNV1A( szFieldName ) ); \
	return *reinterpret_cast< std::add_pointer_t< tReturn > >( reinterpret_cast< std::uintptr_t >( this ) + unOffset + nAdditional ); \
}

#define SCHEMA_VARIABLE( szClassName, szFieldName, szFunctionName, tReturn... ) \
	SCHEMA_VARIABLE_OFFSET( szClassName, szFieldName, szFunctionName, 0, tReturn )