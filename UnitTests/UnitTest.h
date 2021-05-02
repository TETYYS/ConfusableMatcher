#pragma once

#include <iostream>
#include <utility>
#include <cute/cute.h>
#include <ConfusableMatcher.h>

void AssertMatch(CMReturn In, size_t Start, size_t Size)
{
	ASSERT_EQUAL(MATCH, In.Status);
	ASSERT_EQUAL(Start, In.Start);
	ASSERT_EQUAL(Size, In.Size);
}

void AssertMatchMulti(CMReturn In, std::vector<size_t> Start, std::vector<size_t> Size, CM_RETURN_STATUS Status = MATCH)
{
	ASSERT_EQUAL(Status, In.Status);
	ASSERT_EQUAL(Start.size(), Size.size());

	bool ok = false;
	for (auto x = 0;x < Start.size();x++) {
		if (Start[x] == In.Start && Size[x] == In.Size) {
			ok = true;
			break;
		}
	}

	ASSERT(ok);
}

void AssertNoMatch(CMReturn In)
{
	ASSERT_EQUAL(NO_MATCH, In.Status);
}