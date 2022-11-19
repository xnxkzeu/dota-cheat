#pragma once

#include "source2_engine/CGameEntitySystem.hpp"

#include "helpers/schema.hpp"
#include "helpers/memory.hpp"

#include "types/color_t.hpp"

#include "../../context/mem.hpp"

class CGameSceneNode;

class CEntityInstance
{
public:
	SCHEMA_VARIABLE( "CEntityInstance", "m_pEntity", GetEntityIdentity, CEntityIdentity* );
};

class C_BaseEntity : public CEntityInstance
{
public:
	SCHEMA_VARIABLE( "C_BaseEntity", "m_pGameSceneNode", GetGameSceneNode, CGameSceneNode* );
	SCHEMA_VARIABLE( "C_BaseEntity", "m_iHealth", GetHealth, int );
	SCHEMA_VARIABLE( "C_BaseEntity", "m_iMaxHealth", GetMaxHealth, int );
	SCHEMA_VARIABLE( "C_BaseEntity", "m_iTeamNum", GetTeam, int );
};

class C_BaseModelEntity : public C_BaseEntity
{
public:
	void OnColorChanged( )
	{
		auto* const pfnOnColorChanged = CTX::Memory->C_BaseModelEntity.pfnOnColorChanged.Cast< void( __thiscall* )( void* ) >( );

		return pfnOnColorChanged( this );
	}

	SCHEMA_VARIABLE( "C_BaseModelEntity", "m_clrRender", GetRenderColor, Color_t )
};

class C_DOTA_BaseNPC : public C_BaseModelEntity
{
public:
	SCHEMA_VARIABLE( "C_DOTA_BaseNPC", "m_flMana", GetMana, float );
	SCHEMA_VARIABLE( "C_DOTA_BaseNPC", "m_bIsIllusion", IsIllusion, bool );
	SCHEMA_VARIABLE( "C_DOTA_BaseNPC", "m_flStartSequenceCycle", GetStartSequenceCycle, float );
};

class C_DOTA_BaseNPC_Hero : public C_DOTA_BaseNPC
{
public:
	SCHEMA_VARIABLE( "C_DOTA_BaseNPC_Hero", "m_hReplicatingOtherHeroModel", GetReplicatingHeroHandle, CEntityHandle );
};

class CBasePlayerController : public C_BaseEntity
{
public:
	SCHEMA_VARIABLE( "CBasePlayerController", "m_bIsLocalPlayerController", GetIsLocalPlayerController, bool );
};

class C_DOTAPlayerController : public CBasePlayerController
{
public:
	SCHEMA_VARIABLE( "C_DOTAPlayerController", "m_hAssignedHero", GetAssignedHeroHandle, CEntityHandle );
};