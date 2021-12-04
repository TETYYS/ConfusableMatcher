#pragma once

#include "MatchingState.h"

enum CMDebugLevel
{
	DEBUG_LEVEL_NONE,
	DEBUG_LEVEL_FAILURES
};

struct MatchingStateDebugFailures : MatchingState
{
	std::vector<CMDebugFailure>* Failures;

	MatchingStateDebugFailures(size_t InPos, size_t ContainsPos, std::pair<size_t, size_t> LastMatched, std::vector<CMDebugFailure>* Failures)
		: MatchingState(InPos, ContainsPos, LastMatched)
	{
		this->Failures = Failures;
	}

public:
	void AddFailure(uint64_t InPos, uint64_t ContainsPos, CM_DEBUG_FAILURE_REASON Reason)
	{
		if (this->Failures->size() >= 1000)
			return;

		CMDebugFailure failure{ InPos, ContainsPos, Reason };
		this->Failures->push_back(failure);
	}
};

template<class T>
struct IsDebuggingFailuresImpl {};
template<>
struct IsDebuggingFailuresImpl<MatchingState> : std::false_type {};
template<>
struct IsDebuggingFailuresImpl<MatchingStateDebugFailures> : std::true_type {};

template <typename T>
constexpr bool IsDebuggingFailures = IsDebuggingFailuresImpl<T>::value;