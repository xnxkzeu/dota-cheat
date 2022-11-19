#include "../hooks.hpp"

#include "../../features/visuals.hpp"

int64_t HookHandler_t::OnCBasePlayerControllerDtor(void* pPlayerControllerInstance, char a2)
{
	if (pVisuals->m_pLocalPlayerController == pPlayerControllerInstance)
	{
		pVisuals->m_pLocalPlayerController = nullptr;
		pVisuals->m_pLocalHero = nullptr;
	}

	return Hooks::Get().m_pfnOnCBasePlayerControllerDtor(pPlayerControllerInstance, a2);
}