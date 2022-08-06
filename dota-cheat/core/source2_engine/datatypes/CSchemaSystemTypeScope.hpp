#pragma once

#include "CUtlTSHash.hpp"

#include "../../helpers/memory.hpp"

class CSchemaType
{
public:
	OFFSET( 0x8, GetTypeName, const char* );
};

class CSchemaField
{
public:
	const char* m_pszFieldName;
	CSchemaType* m_pSchemaType;
	std::uint32_t m_nOffset;
private:
	[[ maybe_unused ]]
	std::byte m_pad0[ 0xC ];
};

class CSchemaClassBindingBase
{
public:
	OFFSET( 0x8, GetName, const char* );
	OFFSET( 0x10, GetProjectName, const char* );
	OFFSET( 0x18, GetSize, std::uint32_t );
	OFFSET( 0x1C, GetNumFields, std::uint16_t );
	OFFSET( 0x28, GetFields, CSchemaField* );
};

class CSchemaSystemTypeScope
{
public:
	OFFSET( 0x8, GetName, char[ 256 ] );
	OFFSET( 0x558, GetBindingsTable, CUtlTSHash< CSchemaClassBindingBase*, 256, unsigned int > );
};