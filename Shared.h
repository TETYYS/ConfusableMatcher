#pragma once

#include <cstdint>

typedef struct _CMOptions
{
	bool MatchRepeating;
	uint64_t StartIndex;
	bool StartFromEnd;
	uint64_t TimeoutNs;
	bool MatchOnWordBoundary;
	void *ContainsPosPointers;
} CMOptions;

typedef enum _CM_RETURN_STATUS
{
	MATCH = 0,
	NO_MATCH = 1,
	TIMEOUT = 2,
	WORD_BOUNDARY_FAIL_START = 3,
	WORD_BOUNDARY_FAIL_END = 4
} CM_RETURN_STATUS;

typedef struct _CMReturn
{
	uint64_t Start;
	uint64_t Size;
	CM_RETURN_STATUS Status;
} CMReturn;