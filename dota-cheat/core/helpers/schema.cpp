#include <unordered_map>
#include <array>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <format>

#include "schema.hpp"
#include "hash.hpp"

#include "../source2_engine/datatypes/CSchemaSystemTypeScope.hpp"

#include "../source2_engine/ISchemaSystem.hpp"

#include "../../context/dota.hpp"

namespace Schema
{
	static constexpr std::array arrModules = std::to_array< const char* >( {
			CLIENT_DLL,
			ENGINE2_DLL
		} );

	std::unordered_map< Hash_t, std::unordered_map< Hash_t, std::uint32_t > > mapOffsets;

	void Setup( )
	{
		for ( auto* const szModuleName : arrModules )
		{
			CSchemaSystemTypeScope* pModuleScope = CTX::DOTA->m_pSchemaSystem->FindTypeScopeForModule( szModuleName );
			if ( !pModuleScope )
				throw std::runtime_error( std::format( "Failed to find type scope for module \"{}\".", szModuleName ) );

			const auto pElements = std::make_unique_for_overwrite< UtlTSHashHandle_t[ ] >( pModuleScope->GetBindingsTable( ).Count( ) );
			const auto nElements = pModuleScope->GetBindingsTable( ).GetElements( 0, pModuleScope->GetBindingsTable( ).Count( ), pElements.get( ) );

			for ( int nElementIndex = 0; nElementIndex < nElements; nElementIndex++ )
			{
				const UtlTSHashHandle_t hElement = pElements[ nElementIndex ];
				if ( !hElement )
					continue;

				CSchemaClassBindingBase* const pClassBinding = pModuleScope->GetBindingsTable( ).Element( hElement );
				if ( !pClassBinding )
					continue;

				const Hash_t ullClassHashed = Hash::FNV1A( pClassBinding->GetName( ) );

				for ( int nFieldIndex = 0; nFieldIndex < pClassBinding->GetNumFields( ); nFieldIndex++ )
				{
					CSchemaField* const pSchemaField = &pClassBinding->GetFields( )[ nFieldIndex ];
					if ( !pSchemaField )
						continue;

					const Hash_t ullFieldHashed = Hash::FNV1A( pSchemaField->m_pszFieldName );

					mapOffsets[ ullClassHashed ][ ullFieldHashed ] = pSchemaField->m_nOffset;
				}
			}
		}
	}

	std::uint32_t Get( Hash_t ullClassHashed, Hash_t ullFieldHashed )
	{
		return mapOffsets[ ullClassHashed ][ ullFieldHashed ];
	}
}