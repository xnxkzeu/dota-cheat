#pragma once

#include <string_view>

#include "memory.hpp"

namespace Interfaces
{
	Memory::Address_t Get( std::string_view szModuleName, std::string_view szInterfaceName );
	Memory::Address_t GetGameSystem( std::string_view szGameSystemName );
}