#include "hash.hpp"

namespace Hash
{
	Hash_t FNV1A( std::string_view szText )
	{
		Hash_t hashValue = FNV1A_BASIS;

		for ( const char cSymbol : szText )
		{
			hashValue ^= cSymbol;
			hashValue *= FNV1A_PRIME;
		}

		return hashValue;
	}

	Hash_t FNV1A( std::wstring_view wszText )
	{
		Hash_t hashValue = FNV1A_BASIS;

		for ( const wchar_t wcSymbol : wszText )
		{
			hashValue ^= wcSymbol;
			hashValue *= FNV1A_PRIME;
		}

		return hashValue;
	}
}