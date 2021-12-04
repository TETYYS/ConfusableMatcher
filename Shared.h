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

typedef enum _CM_DEBUG_FAILURE_REASON
{
	DEBUG_FAILURE_REASON_NO_PATH = 0,
	DEBUG_FAILURE_REASON_NO_NEW_PATHS = 1,
	DEBUG_FAILURE_REASON_TIMEOUT = 2,
	DEBUG_FAILURE_REASON_WORD_BOUNDARY_FAIL_START = 3,
	DEBUG_FAILURE_REASON_WORD_BOUNDARY_FAIL_END = 4
} CM_DEBUG_FAILURE_REASON;

typedef struct _CMDebugFailure
{
	uint64_t InPos;
	uint64_t ContainsPos;
	CM_DEBUG_FAILURE_REASON Reason;
} CMDebugFailure;

typedef struct _CMDebugFailures
{
	CMDebugFailure* Failures;
	uint64_t Size;
} CMDebugFailures;