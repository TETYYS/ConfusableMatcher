#pragma once

#include <cstdint>

typedef struct _CMOptions
{
	bool MatchRepeating;
	std::size_t StartIndex;
	bool StartFromEnd;
	std::size_t StatePushLimit;
	bool MatchOnWordBoundary;
} CMOptions;