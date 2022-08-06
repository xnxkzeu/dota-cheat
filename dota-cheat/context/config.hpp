#pragma once

#include <optional>

namespace CTX
{
	struct Config_t
	{
		struct Visuals_t
		{
			bool m_bHighlighIllusions = false;
		} Visuals;
	};

	inline std::optional< Config_t > Config = std::nullopt; // @note xnxkzeu: std::optional is used here to delay initialization.
}