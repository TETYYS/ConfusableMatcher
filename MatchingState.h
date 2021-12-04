#pragma once

#include <utility>

struct MatchingState
{
	size_t InPos;
	size_t ContainsPos;
	std::pair<size_t, size_t> LastMatched;

	MatchingState(size_t InPos, size_t ContainsPos, std::pair<size_t, size_t> LastMatched)
	{
		this->InPos = InPos;
		this->ContainsPos = ContainsPos;
		this->LastMatched = LastMatched;
	}
};