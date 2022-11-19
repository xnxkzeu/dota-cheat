#pragma once

#include <memory>

class C_DOTA_BaseNPC_Hero;
class C_DOTAPlayerController;

class CVisuals
{
public:
	CVisuals( ) = default;
	~CVisuals( ) = default;

	void OnRender( );

private:
	void HighlightIllusions( C_DOTA_BaseNPC_Hero* pHero ) const;
	void VisibleByEnemy( ) const;

public:
	C_DOTA_BaseNPC_Hero* m_pLocalHero = nullptr;
	C_DOTAPlayerController* m_pLocalPlayerController = nullptr;
};

inline std::unique_ptr< CVisuals > pVisuals = nullptr;