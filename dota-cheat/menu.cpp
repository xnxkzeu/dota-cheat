#include <imgui.h>

#include "menu.hpp"

#include "core/helpers/input.hpp"

#include "context/dota.hpp"
#include "context/config.hpp"

namespace Menu
{
	bool bIsVisible = false;

	ImGuiIO* pIO = nullptr;

	void Setup( )
	{
		pIO = &ImGui::GetIO( );
		pIO->ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		Input::RegisterCallback( VK_INSERT, Toggle );
	}

	void Render( )
	{
		if ( !bIsVisible )
			return;

		if( ImGui::Begin( PROJECT_NAME ) )
		{
			ImGui::Checkbox( "Highlight Illusions", &CTX::Config->Visuals.m_bHighlighIllusions );
		}
		ImGui::End( );
	}

	void Toggle( )
	{
		bIsVisible = !bIsVisible;
	}

	bool IsVisible( )
	{
		return bIsVisible;
	}
}