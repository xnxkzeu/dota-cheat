#include "visuals.hpp"

#include "../core/source2_engine/CGameEntitySystem.hpp"
#include "../core/source2_engine/IVEngineClient2.hpp"

#include "../core/entities.hpp"

#include "../context/dota.hpp"
#include "../context/config.hpp"

void CVisuals::OnRender( )
{
	for ( int nEntityIndex = 0, nEntityHighestIndex = CTX::DOTA->m_pEntitySystem->GetHighestEntityIndex( );
		nEntityIndex <= nEntityHighestIndex;
		nEntityIndex++ )
	{
		C_BaseEntity* pEntity = CTX::DOTA->m_pEntitySystem->GetBaseEntity( nEntityIndex );
		if ( !pEntity )
			continue;

		CEntityIdentity* pEntityIdentity = pEntity->GetEntityIdentity( );
		if ( !pEntityIdentity || !pEntityIdentity->m_pszEntityName )
			continue;

		if ( !strcmp(pEntityIdentity->m_pszEntityName, "dota_player_controller") )
		{
			if ( C_DOTAPlayerController* pPlayerController = static_cast< C_DOTAPlayerController* >( pEntity );
				pPlayerController->GetIsLocalPlayerController() )
			{
				m_pLocalPlayerController = pPlayerController;
				m_pLocalHero = static_cast< C_DOTA_BaseNPC_Hero* >( CTX::DOTA->m_pEntitySystem->GetBaseEntityFromHandle( m_pLocalPlayerController->GetAssignedHeroHandle( ) ) );
			}
			continue;
		}

		if ( !m_pLocalHero
			|| strstr( pEntityIdentity->m_pszEntityName, "npc_dota_hero" ) != pEntityIdentity->m_pszEntityName )
			continue;

		C_DOTA_BaseNPC_Hero* pHero = static_cast< C_DOTA_BaseNPC_Hero* >( pEntity );
		if ( !pHero || pHero->GetTeam( ) == m_pLocalHero->GetTeam( ) )
			continue;

		HighlightIllusions( pHero );
	}

	if ( !m_pLocalHero )
		return;

	VisibleByEnemy( );
}

void CVisuals::HighlightIllusions( C_DOTA_BaseNPC_Hero* pHero ) const
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
	else if ( clrRender == ILLUSIONS_COLOR )
	{
		clrRender = Color_t( 255, 255, 255, 255 );
		pHero->OnColorChanged( );
	}

}

void CVisuals::VisibleByEnemy( ) const
{
	static constexpr Color_t VISIBLE_COLOR = Color_t( 255, 0, 0, 255 );

	Color_t& clrRender = m_pLocalHero->GetRenderColor( );

	if ( CTX::Config->Visuals.m_bVisibleByEnemy 
		&& m_pLocalHero->GetStartSequenceCycle( ) == 0.f )
	{
		if ( clrRender != VISIBLE_COLOR )
		{
			clrRender = VISIBLE_COLOR;
			m_pLocalHero->OnColorChanged( );
		}
	}
	else if ( clrRender == VISIBLE_COLOR )
	{
		clrRender = Color_t( 255, 255, 255, 255 );
		m_pLocalHero->OnColorChanged( );
	}
}