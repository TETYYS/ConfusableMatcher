#include <iostream>
#include <chrono>
#include <stack>
#include <stdexcept>
#include <plf_stack.h>
#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
#include "ConfusableMatcher.h"

std::bitset<1114110> ConfusableMatcher::WordBoundaries;
bool ConfusableMatcher::Initialized;

bool ConfusableMatcher::AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate)
{
	if (Key.size() == 0)
		throw std::runtime_error("Key.size() == 0");
	if (Value.size() == 0)
		throw std::runtime_error("Value.size() == 0");
	if (Key[0] == '\x00' || Key[0] == '\x01')
		throw std::runtime_error("Key cannot begin with \x00 or \x01");
	if (Value[0] == '\x00' || Value[0] == '\x01')
		throw std::runtime_error("Value cannot begin with \x00 or \x01");

	std::vector<
		std::pair<
			CMString, // Key whole
			CMInnerHashMap*
		>*
	>* keyArr;

	auto findKeyArr = TheMap->find(Key[0]);

	if (findKeyArr == TheMap->end()) {
		// Root doesn't exist
		keyArr = new std::vector<
			std::pair<
				CMString, // Key whole
				CMInnerHashMap*
			>*
		>;
		(*TheMap)[Key[0]] = keyArr;
	} else
		keyArr = findKeyArr->second;

	CMInnerHashMap *valDict = nullptr;

	for (auto it = keyArr->begin();it != keyArr->end();it++) {
		if ((*it)->first.View() == Key) {
			valDict = (*it)->second;
			break;
		}
	}

	if (valDict == nullptr) {
		// Value dictionary doesn't exist
		valDict = new CMInnerHashMap;
		valDict->set_empty_key('\x0');
		valDict->set_deleted_key('\x1');

		auto key = CMString::CopyFromString(Key);

		keyArr->push_back(new std::pair<CMString, CMInnerHashMap*>(key, valDict));
	}

	std::vector<CMString> *valArr;
	auto findValArr = valDict->find(Value[0]);
	if (findValArr == valDict->end()) {
		// Value array doesn't exist
		valArr = new std::vector<CMString>;
		(*valDict)[Value[0]] = valArr;
	} else
		valArr = findValArr->second;

	if (CheckValueDuplicate) {
		for (auto it = valArr->begin();it != valArr->end();it++) {
			if ((*it).View() == Value) {
				return false;
			}
		}
	}

	auto value = CMString::CopyFromString(Value);

	valArr->push_back(value);
	return true;
}

bool ConfusableMatcher::AddSkip(std::string In)
{
	auto in = CMString::CopyFromString(In);

	auto found = SkipSet[(unsigned char)In[0]];

	if (found == nullptr) {
		SkipSet[(unsigned char)In[0]] = found = new std::vector<CMString>();
	}

	for (auto it = found->begin();it != found->end();it++) {
		if ((*it).View() == In) {
			return false;
		}
	}

	found->push_back(in);
	return true;
}

ConfusableMatcher::ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap, std::unordered_set<std::string> Skip, bool AddDefaultValues)
{
	if (!ConfusableMatcher::Initialized)
	{
		ConfusableMatcher::Init();
		ConfusableMatcher::Initialized = true;
	}

	TheMap = new google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				CMString, // Key whole
				CMInnerHashMap*
			>*
		>*,
		std::hash<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<std::pair<CMString, CMInnerHashMap*>*>*>>
	>;
	TheMap->set_empty_key(U'\x0');
	TheMap->set_deleted_key(U'\x1');

	if (AddDefaultValues) {
		// Add some default values
		for (auto x = 'A';x <= 'Z';x++) {
			AddMapping(std::string(1, (char)x), std::string(1, (char)x), true);
			AddMapping(std::string(1, (char)x), std::string(1, (char)(x + 0x20)), true);
		}
		for (auto x = '0';x <= '9';x++)
			AddMapping(std::string(1, (char)x), std::string(1, (char)x), true);
	}

	for (auto it = InputMap.begin();it != InputMap.end();it++)
		AddMapping(it->first, it->second, true);

	for (auto x = 0;x < std::extent<decltype(SkipSet)>::value;x++) {
		SkipSet[x] = nullptr;
	}

	for (auto it = Skip.begin();it != Skip.end();it++)
		AddSkip(*it);
}

void ConfusableMatcher::GetKeyMappings(std::string In, StackVector<CMString> &Output)
{
	StackVector<std::pair<std::string, std::string>> mappingsStorage;

	assert(In.size() >= 1);

	// Find array of whole keys
	auto keyArr = TheMap->find(In[0]);
	if (keyArr == TheMap->end())
		return;

	for (auto keyIterator = keyArr->second->begin();keyIterator != keyArr->second->end();keyIterator++) {
		if ((*keyIterator)->first.Len > In.size() || ((*keyIterator)->first).View() != CMStringView(In.data(), (*keyIterator)->first.Len))
			continue;

		// Whole key found, return value array
		auto values = (*keyIterator)->second;

		for (auto valueIterator = values->begin();valueIterator != values->end();valueIterator++) {
			for (auto x = 0;x < valueIterator->second->size();x++) {
				Output.Push((*(valueIterator->second))[x]);
			}
		}
	}
}

CMStringPosPointers *ConfusableMatcher::ComputeStringPosPointers(std::string Contains)
{
	auto ret = new CMStringPosPointers();

	// Loop for each char
	for (auto x = 0;x < Contains.size();x++) {
		std::vector<std::pair<size_t /* Key len */, const CMInnerHashMap*>> vec;

		auto keyArr = TheMap->find(Contains[x]);

		// Find and put all inner hashmaps, together with key substring length
		if (keyArr != TheMap->end()) {
			for (auto keyIterator = keyArr->second->begin();keyIterator != keyArr->second->end();keyIterator++) {
				if ((*keyIterator)->first.Len > Contains.size() - x || ((*keyIterator)->first).View() != CMStringView(Contains.data() + x, (*keyIterator)->first.Len))
					continue;

				vec.push_back(std::pair<size_t, const CMInnerHashMap*>((*keyIterator)->first.Len, (*keyIterator)->second));
			}
		}

		// Puts at `x` pos
		ret->PosPointers.push_back(vec);
	}

	return ret;
}

int ConfusableMatcher::StrCompareWithSkips(const CMStringView In, size_t Pos, const CMStringView Compare)
{
	assert(In.size() > Pos);
	assert(In.size() >= Compare.size());

	auto in = In.data() + Pos;
	auto cmp = Compare.data();
	auto cmpEnd = Compare.data() + Compare.size();
	auto inEnd = In.data() + In.size();

	for (;in < inEnd && cmp < cmpEnd;) {
		// Compare regular chars
		if (*in == *cmp) {
			in++;
			cmp++;
			continue;
		}

		// If it didn't match anything, try to skip chars in In
		auto foundSkip = SkipSet[(unsigned char)*in];
		if (foundSkip != nullptr) {
			const auto& vec = *foundSkip;
			for (auto i = 0;i < vec.size();i++) {
				auto el = vec[i];
				auto thisSz = el.Len;

				if (in + thisSz <= inEnd && strncmp(in, el.Str, thisSz) == 0) {
					in += thisSz;
					goto next;
				}
			}
		}

		// Return no match if we exhausted all options
		return -1;
		next:;
	}

	return cmp != cmpEnd ? -1 : (in - In.data() - Pos);
}

void ConfusableMatcher::GetMappings(CMStringPosPointers *PosPointers, size_t Pos, long long ExactSize, CMStringView Value, size_t ValuePos, StackVector<std::pair<size_t, size_t>> &Storage)
{
	assert(Value.size() - ValuePos >= 1);

	// Get CMInnerHashMap for provided key by pos
	const auto& vals = PosPointers->PosPointers[Pos];

	for (auto x = 0;x < vals.size();x++) {
		const auto inner = vals[x];

		// For if we are looking for specific substring of key
		if (ExactSize != -1 && inner.first != ExactSize)
			continue;

		// Value searching
		auto foundArr = inner.second->find(Value[ValuePos]);
		if (foundArr == inner.second->end())
			continue;

		const auto& vec = *(foundArr->second);
		auto sz = vec.size();

		for (auto x = 0;x < sz;x++) {
			const auto el = vec[x];

			if (el.Len > Value.size() - ValuePos)
				continue;

			auto valLen = StrCompareWithSkips(Value.data(), ValuePos, el.View());

			if (valLen != -1)
				Storage.Push(std::pair<size_t, size_t>(inner.first, valLen));
		}
	}

	return;
}

void ConfusableMatcher::GetMappings(const CMStringView Key, size_t KeyPos, const CMStringView Value, size_t ValuePos, StackVector<std::pair<size_t, size_t>> &Storage)
{
	assert(Key.size() - KeyPos >= 1);
	assert(Value.size() - ValuePos >= 1);

	// Find array of whole keys
	auto keyArr = TheMap->find(Key[KeyPos]);
	if (keyArr == TheMap->end())
		return;

	const auto &vec = *(keyArr->second);

	for (auto x = 0;x < vec.size();x++) {
		const auto item = vec[x];

		if (item->first.Len <= Key.size() - KeyPos && strncmp(item->first.Str, Key.data() + KeyPos, item->first.Len) == 0) {
			// Whole key found, search for value array
			auto foundArr = item->second->find(Value[ValuePos]);
			if (foundArr == item->second->end())
				continue;

			const auto &secondVec = *(foundArr->second);

			for (auto i = 0;i < secondVec.size();i++) {
				const auto secondItem = secondVec[i];

				if (secondItem.Len > Value.size() - ValuePos)
					continue;

				auto valLen = StrCompareWithSkips(Value.data(), ValuePos, secondItem.View());

				if (valLen != -1)
					Storage.Push(std::pair<size_t, size_t>((item->first).Len, valLen)); // Whole value found
			}
		}
	}
}

int ConfusableMatcher::MatchWordBoundary(unsigned char i0)
{
	// Nothing special for 0-255
	return ConfusableMatcher::WordBoundaries[i0];
}

int ConfusableMatcher::MatchWordBoundary(unsigned char i0, unsigned char i1)
{
	// UTF8 decode and find in word boundaries bitset
	auto c = ((i0 & 0x1F) << 6) | (i1 & 0x3F);

	if ((i1 & 0xC0) == 0x80)
		return ConfusableMatcher::WordBoundaries[c] ? 1 : 0;
	return -1;
}

int ConfusableMatcher::MatchWordBoundary(unsigned char i0, unsigned char i1, unsigned char i2)
{
	auto c = ((i0 & 0x0F) << 12) | ((i1 & 0x3F) << 6) | (i2 & 0x3F);

	if ((i1 & 0xC0) == 0x80 && ((i2 & 0xC0) == 0x80))
		return ConfusableMatcher::WordBoundaries[c] ? 1 : 0;
	return -1;
}

int ConfusableMatcher::MatchWordBoundary(unsigned char i0, unsigned char i1, unsigned char i2, unsigned char i3)
{
	// Here also check for length as it can exceed it
	auto c = ((i0 & 0x07) << 18) | ((i1 & 0x3F) << 12) | ((i2 & 0x3F) << 6) | (i3 & 0x3F);
	if ((i1 & 0xC0) == 0x80 && ((i2 & 0xC0) == 0x80))
		return c < ConfusableMatcher::WordBoundaries.size() && ConfusableMatcher::WordBoundaries[c] ? 1 : 0;
	return -1;
}

bool ConfusableMatcher::MatchWordBoundaryToLeft(CMStringView In)
{
	/*
	 * When searching to left from right, it doesn't need to worry about anything special
	 * as first char will be always in the same spot
	 */
	assert(In.size() < 5);
	
	if (In.size() == 0)
		return false;

	/*
	 * 1 for match, 0 for match failure and -1 for UTF8 decode failure
	 * 
	 * For UTF8 decode failure it can try other length codepoints
	 * 
	 * For match failure it must immediately return because codepoint has been consumed
	 * and it is searching for word boundary chars at the start of `In`
	 */
	int res;
#define process(i) { res = (i); if (res == 1) { return true; } else if (res == 0) { return false; } }

	if (In.size() == 4) {
		process(MatchWordBoundary(In[0], In[1], In[2], In[3]));
		process(MatchWordBoundary(In[0], In[1], In[2]));
		process(MatchWordBoundary(In[0], In[1]));
	} else if (In.size() == 3) {
		process(MatchWordBoundary(In[0], In[1], In[2]));
		process(MatchWordBoundary(In[0], In[1]));
	} else if (In.size() == 2)
		process(MatchWordBoundary(In[0], In[1]));

	if (MatchWordBoundary(In[0]))
		return true;

	return false;

#undef process
}

bool ConfusableMatcher::MatchWordBoundaryToRight(CMStringView In)
{
	/*
	 * When searching to right from left, it needs to check everything at most four times as it, for example,
	 * can decode 3 char length codepoint without a match and then find a match at fourth position
	 */
	assert(In.size() < 5);
	
	if (In.size() == 0)
		return false;

	/*
	 * 1 for match, 0 for UTF8 decode but match failure and -1 for UTF8 decode failure
	 * 
	 * For match failure it needs to skip whole char as codepoint is consumed and try matching unconsumed part again
	 * 
	 * For UTF8 decode failure it can still try decoding codepoints that touch the right as it doesn't
	 * need word boundary chars that are not actually touching the match string
	 */
	int res;
#define process(i, x) { res = (i); if (res == 1) { return (x); } else if (res == 0) { return MatchWordBoundaryToRight(CMStringView(In.data() + x, In.size() - x)); } }

	if (In.size() == 4) {
		process(MatchWordBoundary(In[0], In[1], In[2], In[3]), 4);
		process(MatchWordBoundary(In[1], In[2], In[3]), 3);
		process(MatchWordBoundary(In[2], In[3]), 2);
		if (MatchWordBoundary(In[3]))
			return true;
	} else if (In.size() == 3) {
		process(MatchWordBoundary(In[0], In[1], In[2]), 3);
		process(MatchWordBoundary(In[1], In[2]), 2);
		if (MatchWordBoundary(In[2]))
			return true;
	} else if (In.size() == 2) {
		process(MatchWordBoundary(In[0], In[1]), 2);
		if (MatchWordBoundary(In[1]))
			return true;
	} else {
		if (MatchWordBoundary(In[0]))
			return true;
	}

	return false;

#undef process
}

CM_RETURN_STATUS ConfusableMatcher::CheckWordBoundary(CMStringView In, CMStringView Match)
{
	bool startPass = false, endPass = false;

	if (In.data() == Match.data()) {
		// Check if it is at the beginning of In
		startPass = true;
	} else {
		// Check for word boundary char that is touching left of Match, thus matching from left to right
		auto distToStart = std::min((size_t)4 /* Max 4 chars of largest codepoint */, (size_t)(Match.data() - In.data()));
		startPass = ConfusableMatcher::MatchWordBoundaryToRight(CMStringView(Match.data() - distToStart, distToStart));
	}

	if (!startPass) {
		return WORD_BOUNDARY_FAIL_START;
	}

	if (In.data() + In.size() == Match.data() + Match.size()) {
		// Check if it is at the end of In
		endPass = true;
	} else {
		// Check for word boundary char that is touching right of Match, thus matching from right to left
		auto matchEnd = (size_t)(Match.data() + Match.size());
		auto distToEnd = std::min((size_t)4 /* Max 4 chars of largest codepoint */, (size_t)In.data() + In.size() - matchEnd);
		endPass = ConfusableMatcher::MatchWordBoundaryToLeft(CMStringView(Match.data() + Match.size(), distToEnd));
	}

	if (!endPass) {
		return WORD_BOUNDARY_FAIL_END;
	}

	return MATCH;
}

CMReturn ConfusableMatcher::IndexOfInner(const CMStringView In, const CMStringView Contains, int StartingIndex, MatchingState State, const std::chrono::steady_clock::time_point Start, const CMOptions Options)
{
	plf::stack<MatchingState> matchingStates;
	StackVector<std::pair<size_t, size_t>> mappingsStorage;
	CMReturn ret;
	ret.Start = StartingIndex;
	ret.Status = NO_MATCH;
	uint64_t statePops = 0;

	auto handleEmptyMatchingStates = [this, &State, Options, &ret, In, Contains, StartingIndex]()
	{
		if (State.ContainsPos == Contains.size()) {
			if (Options.MatchOnWordBoundary) {
				ret.Status = CheckWordBoundary(In, CMStringView(In.data() + StartingIndex, State.InPos - StartingIndex));
			} else
				ret.Status = MATCH;

			ret.Size = State.InPos - StartingIndex;
		}

		return ret;
	};


	while (true) {
		if (State.InPos == In.size()) {
			// Current state ate all of `In` and we have no other state to get from stack
			if (matchingStates.empty()) {
				return handleEmptyMatchingStates();
			}

			// This happens when there are still matches in stack but current state ate all of `In`
			State = matchingStates.top();
			matchingStates.pop();
			continue;
		}
		
		if (Options.MatchRepeating) {
			// Try to match repeating substring
			mappingsStorage.Reset();
			if (Options.ContainsPosPointers == nullptr)
				GetMappings(CMStringView(Contains.data(), State.LastMatched.second + State.LastMatched.first), State.LastMatched.first, In, State.InPos, mappingsStorage);
			else
				GetMappings((CMStringPosPointers*)Options.ContainsPosPointers, State.LastMatched.first, State.LastMatched.second, In, State.InPos, mappingsStorage);
			auto mappingsSize = mappingsStorage.Size();

			if (mappingsSize != 0) {
				// Push every new matching path
				for (auto x = 0;x < mappingsStorage.Size();x++) {
					const auto& item = mappingsStorage.GetElement(x);

					matchingStates.push(MatchingState(
						State.InPos + item.second,
						State.ContainsPos,
						State.LastMatched
					));
				}
			}
		}

		// Try to skip specified substrings
		auto foundSkip = SkipSet[(unsigned char)In[State.InPos]];
		if (foundSkip != nullptr) {
			const auto& vec = *foundSkip;
			for (auto x = 0;x < vec.size();x++) {
				auto el = vec[x];
				auto thisSz = el.Len;

				if (State.InPos + thisSz <= In.size() && strncmp(In.data() + State.InPos, el.Str, thisSz) == 0) {
					// Push new path

					matchingStates.push(MatchingState(
						State.InPos + thisSz,
						State.ContainsPos,
						State.LastMatched
					));
				}
			}
		}

		// Try to match next substring
		size_t mappingsSize = 0;
		if (State.ContainsPos != Contains.size()) {
			mappingsStorage.Reset();
			if (Options.ContainsPosPointers == nullptr)
				GetMappings(Contains, State.ContainsPos, In, State.InPos, mappingsStorage);
			else
				GetMappings((CMStringPosPointers*)Options.ContainsPosPointers, State.ContainsPos, -1, In, State.InPos, mappingsStorage);
			mappingsSize = mappingsStorage.Size();
		}

		if (mappingsSize != 0) {
			for (auto x = 0;x < mappingsStorage.Size();x++) {
				const auto& item = mappingsStorage.GetElement(x);

				// If we were about to eat all of `Contains`, that means we found the whole thing
				if (item.first == Contains.size() - State.ContainsPos) {
					// But check for word boundary first
					ret.Size = State.InPos + item.second - StartingIndex;

					if (Options.MatchOnWordBoundary)
						ret.Status = CheckWordBoundary(In, CMStringView(In.data() + StartingIndex, ret.Size));
					else
						ret.Status = MATCH;

					if (ret.Status == MATCH)
						return ret;

					/*
					 * We were about to eat the whole thing but it turned out to be not what we need.
					 * What we can still do is skip chars and match repeating
					 */ 
				}

				// Push new path
				matchingStates.push(MatchingState(
					State.InPos + item.second,
					State.ContainsPos + item.first,
					std::pair<size_t, size_t>(State.ContainsPos, item.first)
				));
			}
		} 
		if (mappingsSize == 0) {
			// We didn't put any new paths into stack so check if we depleted all paths
			if (matchingStates.empty()) {
				return handleEmptyMatchingStates();
			}
		}

		// Fetch newest path --- this includes last new path from next substring matching
		State = matchingStates.top();
		matchingStates.pop();
		statePops++;

		if (statePops % 100 == 0 && (std::chrono::steady_clock::now() - Start).count() > Options.TimeoutNs) {
			ret.Size = State.InPos - StartingIndex;
			ret.Status = TIMEOUT;
			return ret;
		}
	}
}

CMReturn ConfusableMatcher::IndexOfFromView(CMStringView In, CMStringView Contains, CMOptions Options)
{
	StackVector<std::pair<size_t, size_t>> MappingsStorage;
	CMReturn ret;
	ret.Start = 0;
	ret.Size = 0;
	ret.Status = NO_MATCH;

	auto start = std::chrono::steady_clock::now();

	/*
	 * No need to check for `In` size, loop never executes due it's size and returns a proper result:
	 *  In(1), Contains(0) returns a match due to check in IndexOf
	 *  In(0), Contains(1) returns no match due to return at the end of the method
	 *  In(0), Contains(0) returns a match due to check in IndexOf (nothing does contain nothing)
	 */

	for (int x = Options.StartIndex;Options.StartFromEnd ? (x >= 0) : (x < In.size());x += Options.StartFromEnd ? -1 : 1) {
		// Find the beginning and construct state from it

		if (Options.ContainsPosPointers == nullptr)
			GetMappings(Contains, 0, In, x, MappingsStorage);
		else
			GetMappings((CMStringPosPointers*)Options.ContainsPosPointers, 0, -1, In, x, MappingsStorage);
		if (MappingsStorage.Size() == 0)
			continue;

		for (auto i = 0;i < MappingsStorage.Size();i++) {
			const auto& item = MappingsStorage.GetElement(i);

			// Matched full string and no repeats
			if (item.first == Contains.size() && x + item.second == In.size()) {
				if (Options.MatchOnWordBoundary)
					ret.Status = CheckWordBoundary(In, CMStringView(In.data() + x, item.second));
				else
					ret.Status = MATCH;

				ret.Start = x;
				ret.Size = item.second;

				if (ret.Status == MATCH) {
					return ret;
				} else {
					assert(ret.Status != WORD_BOUNDARY_FAIL_END);
					continue;
				}
			}

			auto result = IndexOfInner(
				In,
				Contains,
				x,
				MatchingState(
					x + item.second,
					item.first,
					std::pair<size_t, size_t>(0, item.first)
				),
				start,
				Options
			);

			if (result.Status == NO_MATCH) {
				if ((std::chrono::steady_clock::now() - start).count() > Options.TimeoutNs) {
					ret.Start = x;
					ret.Size = 1;
					ret.Status = TIMEOUT;
					return ret;
				}
				continue;
			} else if (result.Status == WORD_BOUNDARY_FAIL_START) {
				// Special case for word boundary matching - we need to check for skips in the beginning
				int skipBlockStart = -1, skipBlockLen = 0;
				for (int y = 0;y < result.Start;y++) {
					bool modifiedSkipBlockLen = false;

					auto foundSkip = SkipSet[(unsigned char)In[y]];
					if (foundSkip != nullptr) {
						const auto& vec = *foundSkip;
						for (auto it = vec.begin();it != vec.end();it++) {
							auto thisSz = it->Len;

							if (y + thisSz <= In.size() && strncmp(In.data() + y, it->Str, thisSz) == 0) {
								modifiedSkipBlockLen = true;
								if (skipBlockStart == -1) {
									skipBlockStart = y;
								}
								skipBlockLen += thisSz;

								if (skipBlockStart + skipBlockLen == x) {
									/*
									 * Approached where we started to match, check if skip block:
									 * ...is at the start of the Input
									 */
									if (skipBlockStart == 0) {
										ret.Start = skipBlockLen;
										ret.Size = result.Size;
										ret.Status = MATCH;
										return ret;
									}

									/*
									 * ...has a word boundary char right before the block
									 * 
									 * Search from start of skip block for word boundary touching the start of skip block (4 to account for longest word boundary string)
									 * 
									 * Need to adjust length 4 times to allow MatchWordBoundaryToRight to properly identify boundaries
									 * 
									 * Ex.:
									 *            __ Touching here
									 * ------|WBWB||SKIPSKIPSKIP|MATCHMATCH|----
									 * 
									 * Or more common case:
									 *            __ Touching here
									 * ---------|W||SKIPSKIPSKIP|MATCHMATCH|----
									 */
									for (auto sbs = 3;sbs >= 0;sbs--) {
										if (skipBlockStart > sbs && ConfusableMatcher::MatchWordBoundaryToRight(CMStringView(In.data() + skipBlockStart - (sbs+1), sbs+1))) {
											ret.Start = skipBlockStart + skipBlockLen;
											ret.Size = result.Size;
											ret.Status = MATCH;
											return ret;
										}
									}

									/*
									 * There are no other possibilies to match as we have a contiguous skip block, so:
									 * • all word boundaries are before it
									 * • there are no word boundaries inside skip block
									 * • there are no word boundaries touching between skip block and actual match, as we wouldn't get WORD_BOUNDARY_FAIL_START in the first place
									 */
								}
							}
						}
					}

					if (!modifiedSkipBlockLen) {
						skipBlockStart = -1;
						skipBlockLen = 0;
					}
				}

				ret.Start = result.Start;
				ret.Size = result.Size;
				ret.Status = result.Status;
			} else if (result.Status == WORD_BOUNDARY_FAIL_END) {
				// Prefer word boundary failure results as they probably make more sense for human
				ret.Start = result.Start;
				ret.Size = result.Size;
				ret.Status = result.Status;
			} else {
				// No need to sync fields to ret as result already has everything to properly return
				return result;
			}
		}

		MappingsStorage.Reset();
	}

	return ret;
}

CMReturn ConfusableMatcher::IndexOf(std::string In, std::string Contains, CMOptions Options)
{
	if (Contains.size() == 0) {
		CMReturn ret;

		ret.Status = MATCH;
		ret.Start = 0;
		ret.Size = 0;
		return ret;
	}

	if (Options.StartIndex >= In.size() || Options.StartIndex < 0) {
		CMReturn ret;
		ret.Status = NO_MATCH;
		ret.Start = 0;
		ret.Size = 0;
		return ret;
	}

	return IndexOfFromView(CMStringView(In), CMStringView(Contains), Options);
}

void ConfusableMatcher::FreeStringPosPointers(CMStringPosPointers *In)
{
	delete In;
}

ConfusableMatcher::~ConfusableMatcher()
{

	if (TheMap->size() != 0) {
		for (auto keyMapIt = TheMap->begin();keyMapIt != TheMap->end();keyMapIt++) {
			for (auto keyArrayIt = keyMapIt->second->begin();keyArrayIt != keyMapIt->second->end();keyArrayIt++) {
				(*keyArrayIt)->first.Free();
				for (auto valMapIt = (*keyArrayIt)->second->begin();valMapIt != (*keyArrayIt)->second->end();valMapIt++) {
					for (auto valArrayIt = valMapIt->second->begin();valArrayIt != valMapIt->second->end();valArrayIt++) {
						valArrayIt->Free();
					}
					delete valMapIt->second;
				}
				delete (*keyArrayIt)->second;
				delete *keyArrayIt;
			}
			delete keyMapIt->second;
		}
	}
	delete TheMap;

	for (auto x = 0;x < std::extent<decltype(SkipSet)>::value;x++) {
		if (SkipSet[x] == nullptr)
			continue;

		for (auto it2 = SkipSet[x]->begin();it2 != SkipSet[x]->end();it2++) {
			it2->Free();
		}
		delete SkipSet[x];
	}
}