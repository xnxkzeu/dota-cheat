#pragma once

#include <cstdint>
#include <cstddef>

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS

using CEntityHandle = std::uint32_t;

class C_BaseEntity;

class CEntityIdentity
{
public:
	C_BaseEntity* m_pEntity;
private:
	[[ maybe_unused ]]
	std::byte m_pad0[ 0x8 ];
public:
	CEntityHandle m_hEntity;
	const char* m_pszInternalName;
	const char* m_pszEntityName;
private:
	[[ maybe_unused ]]
	std::byte m_pad1[ 0x28 ];
public:
	CEntityIdentity* m_pPrev;
	CEntityIdentity* m_pNext;
private:
	[[ maybe_unused ]]
	std::byte m_pad2[ 0x18 ];
};

class CEntityIdentities
{
public:
	CEntityIdentity m_pIdentities[ MAX_ENTITIES_IN_LIST ];
};

class CGameEntitySystem
{
public:
	C_BaseEntity* GetBaseEntity( int nIndex )
	{
		if ( nIndex <= -1 || nIndex >= ( MAX_TOTAL_ENTITIES - 1 ) )
			return nullptr;

		CEntityIdentities* pChunkToUse = m_pIdentityChunks[ nIndex / MAX_ENTITIES_IN_LIST ];
		if ( !pChunkToUse )
			return nullptr;

		CEntityIdentity* pIdentity = &pChunkToUse->m_pIdentities[ nIndex % MAX_ENTITIES_IN_LIST ];
		if ( !pIdentity )
			return nullptr;

		return pIdentity->m_pEntity;
	}

	C_BaseEntity* GetBaseEntityFromHandle( CEntityHandle hEntity )
	{
		return GetBaseEntity( static_cast< int >( static_cast< std::uint16_t >( hEntity ) & 0x7FFF ) );
	}

	int GetHighestEntityIndex()
	{
		return *reinterpret_cast< int* >( reinterpret_cast< std::uintptr_t >( this ) + 0x2090 ) + 1;
	}
private:
	[[ maybe_unused ]]
	std::byte m_pad0[ 0x10 ];
public:
	CEntityIdentities* m_pIdentityChunks[ MAX_ENTITY_LISTS ];
};