#include <Windows.h>
#include <stdexcept>
#include <format>

#include "memory.hpp"

namespace Memory
{
	Address_t FindPattern( const std::string_view szModuleName, const std::string_view szPattern )
	{
		const Address_t pModule = GetModuleBaseAddress( szModuleName );

		if ( pModule == nullptr )
			throw std::runtime_error( std::vformat( "Failed to get base address for \"{}\" module.", std::make_format_args( szModuleName ) ) );

		const IMAGE_DOS_HEADER* pDosHeader = pModule.Cast< const IMAGE_DOS_HEADER* >( );
		const IMAGE_NT_HEADERS* pNtHeaders = pModule.Offset< const IMAGE_NT_HEADERS* >( pDosHeader->e_lfanew );

		return FindPattern( pModule, pNtHeaders->OptionalHeader.SizeOfImage, szPattern );

	}

	Address_t FindPattern( const Address_t pRegionStart, const std::size_t unRegionSize, const std::string_view szPattern )
	{
		const std::uint8_t* pBytes = pRegionStart.Cast< std::uint8_t* >( );

		const std::vector< std::optional< std::uint8_t > > vecBytes = PatternToBytes( szPattern );

		for ( std::uintptr_t uIndex = 0, uSize = unRegionSize - vecBytes.size( ); uIndex < uSize; ++uIndex )
		{
			bool bByteFound = true;

			for ( std::uintptr_t uByteIndex = 0; uByteIndex < vecBytes.size( ); ++uByteIndex )
			{
				if ( vecBytes[ uByteIndex ].has_value( ) && pBytes[ uIndex + uByteIndex ] != vecBytes[ uByteIndex ].value( ) )
				{
					bByteFound = false;
					break;
				}
			}

			if ( bByteFound )
				return reinterpret_cast< std::uintptr_t >( &pBytes[ uIndex ] );
		}

		return { };
	}

	Address_t GetModuleBaseAddress( const std::string_view szModuleName )
	{
		return reinterpret_cast< void* >( GetModuleHandleA( szModuleName.data( ) ) );
	}

	Address_t GetExportAddress( const Address_t pModuleBase, const std::string_view szProcedureName )
	{
		const IMAGE_DOS_HEADER* pDosHeader = pModuleBase.Cast< const IMAGE_DOS_HEADER* >( );
		const IMAGE_NT_HEADERS* pNtHeaders = pModuleBase.Offset< const IMAGE_NT_HEADERS* >( pDosHeader->e_lfanew );
		const IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNtHeaders->OptionalHeader;

		const std::uint32_t uExportSize = pOptionalHeader->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
		const std::uint32_t uExportAddress = pOptionalHeader->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

		if ( uExportSize <= 0 )
			return { };

		const IMAGE_EXPORT_DIRECTORY* pExportDirectory = pModuleBase.Offset< const IMAGE_EXPORT_DIRECTORY* >( uExportAddress );
		const std::uint32_t* pNamesRVA = pModuleBase.Offset< const std::uint32_t* >( pExportDirectory->AddressOfNames );
		const std::uint32_t* pFunctionsRVA = pModuleBase.Offset< const std::uint32_t* >( pExportDirectory->AddressOfFunctions );
		const std::uint16_t* pNameOrdinals = pModuleBase.Offset< const std::uint16_t* >( pExportDirectory->AddressOfNameOrdinals );

		std::uintptr_t uRight = pExportDirectory->NumberOfNames;
		std::uintptr_t uLeft = 0;

		while ( uRight != uLeft )
		{
			const std::uintptr_t uMiddle = uLeft + ( ( uRight - uLeft ) >> 1U );
			const int iResult = szProcedureName.compare( pModuleBase.Offset< const char* >( pNamesRVA[ uMiddle ] ) );

			if ( iResult == 0 )
				return pModuleBase.Offset( pFunctionsRVA[ pNameOrdinals[ uMiddle ] ] );

			if ( iResult > 0 )
				uLeft = uMiddle;
			else
				uRight = uMiddle;
		}

		return { };
	}

	std::vector< std::optional< std::uint8_t > > PatternToBytes( const std::string_view szPattern )
	{
		std::vector< std::optional< std::uint8_t > > vecBytes = { };

		auto it = szPattern.cbegin( );
		const auto itEnd = szPattern.cend( );

		while ( it < itEnd )
		{
			if ( *it == '?' )
			{
				if ( ++it; *it == '?' )
					++it;

				vecBytes.emplace_back( std::nullopt );
			}
			else if ( *it != ' ' )
			{
				std::uint8_t uByte = static_cast< std::uint8_t >( ( ( *it >= 'A' ? ( ( ( *it - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 ) : ( *it <= '9' ? *it - '0' : 0x0 ) ) | 0xF0 ) << 4 );

				if ( ++it; *it != ' ' )
					uByte |= static_cast< std::uint8_t >( *it >= 'A' ? ( ( ( *it - 'A' ) & ( ~( 'a' ^ 'A' ) ) ) + 10 ) : ( *it <= '9' ? *it - '0' : 0x0 ) );

				vecBytes.emplace_back( uByte );
			}

			++it;
		}

		return vecBytes;
	}

	std::string BytesToPattern( const std::uint8_t* pBytes, const std::size_t uSize )
	{
		constexpr const char* szHexDigits = "0123456789ABCDEF";
		const std::size_t nHexLength = ( uSize << 1U ) + uSize;

		std::string szPattern( nHexLength - 1U, ' ' );

		for ( std::size_t nIndex = 0U, nByteIndex = 0U; nIndex < nHexLength; nByteIndex++, nIndex += 3U )
		{
			const std::uint8_t uCurrentByte = pBytes[ nByteIndex ];

			szPattern[ nIndex ] = szHexDigits[ ( ( uCurrentByte & 0xF0 ) >> 4 ) ];
			szPattern[ nIndex + 1U ] = szHexDigits[ ( uCurrentByte & 0x0F ) ];
		}

		return szPattern;
	}
}