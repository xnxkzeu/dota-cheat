#pragma once

#include <memory>

class C_DOTA_BaseNPC_Hero;

class CVisuals
{
public:
	CVisuals( ) = default;
	~CVisuals( ) = default;

	void OnRender( );

private:
	void HighlightIllusions( C_DOTA_BaseNPC_Hero* pHero );

	C_DOTA_BaseNPC_Hero* m_pLocalHero = nullptr;
};

inline std::unique_ptr< CVisuals > pVisuals = nullptr;