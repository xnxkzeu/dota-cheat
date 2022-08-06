#include "visuals.hpp"

#include "../core/source2_engine/CGameEntitySystem.hpp"
#include "../core/source2_engine/IVEngineClient2.hpp"

#include "../core/entities.hpp"

#include "../context/dota.hpp"
#include "../context/config.hpp"

void CVisuals::OnRender( )
{
	int nLocalPlayerSlot = -1;
	if ( CTX::DOTA->m_pEngineClient->GetLocalPlayer( nLocalPlayerSlot ); nLocalPlayerSlot == -1 )
		return;

	C_DOTAPlayerController* pLocalController = static_cast< C_DOTAPlayerController* >( CTX::DOTA->m_pEntitySystem->GetBaseEntity( nLocalPlayerSlot + 1 ) );
	if ( !pLocalController )
		return;

	if ( m_pLocalHero = static_cast< C_DOTA_BaseNPC_Hero* >( CTX::DOTA->m_pEntitySystem->GetBaseEntityFromHandle( pLocalController->GetAssignedHeroHandle( ) ) ); !m_pLocalHero )
		return;

	for ( int nEntityIndex = 0, nEntityHighestIndex = CTX::DOTA->m_pEntitySystem->GetHighestEntityIndex( );
		nEntityIndex <= nEntityHighestIndex;
		nEntityIndex++ )
	{
		C_BaseEntity* pEntity = CTX::DOTA->m_pEntitySystem->GetBaseEntity( nEntityIndex );
		if ( !pEntity )
			continue;

		CEntityIdentity* pEntityIdentity = pEntity->GetEntityIdentity( );
		if ( !pEntityIdentity
			|| !pEntityIdentity->m_pszEntityName
			|| strstr( pEntityIdentity->m_pszEntityName, "npc_dota_hero" ) != pEntityIdentity->m_pszEntityName )
			continue;

		C_DOTA_BaseNPC_Hero* pHero = static_cast< C_DOTA_BaseNPC_Hero* >( pEntity );
		if ( !pHero || pHero->GetTeam( ) == m_pLocalHero->GetTeam( ) )
			continue;

		HighlightIllusions( pHero );
	}
}

void CVisuals::HighlightIllusions( C_DOTA_BaseNPC_Hero* pHero )
{
	static constexpr Color_t ILLUSIONS_COLOR = Color_t( 0, 0, 255, 255 );

	if ( pHero->GetReplicatingHeroHandle( ) == std::numeric_limits< CEntityHandle >::max( )
		|| pHero->GetReplicatingHeroHandle( ) == 0 )
		return;

	Color_t& clrRender = pHero->GetRenderColor( );
	if ( CTX::Config->Visuals.m_bHighlighIllusions )
	{
		if ( clrRender != ILLUSIONS_COLOR )
		{
			clrRender = ILLUSIONS_COLOR;
			pHero->OnColorChanged( );
		}
	}
	else
	{
		if ( clrRender == ILLUSIONS_COLOR )
		{
			clrRender = Color_t( 255, 255, 255, 255 );
			pHero->OnColorChanged( );
		}
	}
}
