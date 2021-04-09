#include <iostream>
#include <chrono>
#include <stack>
#include <stdexcept>

#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
#include "ConfusableMatcher.h"

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
			google::dense_hash_map<
				char, // Value first char
				std::vector<CMString>* // Values whole
			>*
		>*
	>* keyArr;

	auto findKeyArr = TheMap->find(Key[0]);

	if (findKeyArr == TheMap->end()) {
		// Root doesn't exist
		keyArr = new std::vector<
			std::pair<
				CMString, // Key whole
				google::dense_hash_map<
					char, // Value first char
					std::vector<CMString>* // Values whole
				>*
			>*
		>;
		(*TheMap)[Key[0]] = keyArr;
	} else
		keyArr = findKeyArr->second;

	google::dense_hash_map<
		char, // Value first char
		std::vector<CMString>* // Values whole
	> *valDict = nullptr;

	for (auto it = keyArr->begin();it != keyArr->end();it++) {
		if ((*it)->first.View() == Key) {
			valDict = (*it)->second;
			break;
		}
	}

	if (valDict == nullptr) {
		// Value dictionary doesn't exist
		valDict = new google::dense_hash_map<
			char, // Value first char
			std::vector<CMString>* // Values whole
		>;
		valDict->set_empty_key('\x0');
		valDict->set_deleted_key('\x1');

		auto key = CMString::CopyFromString(Key);

		keyArr->push_back(new std::pair<CMString, google::dense_hash_map<char, std::vector<CMString>*>*>(key, valDict));
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

	auto found = SkipSet->find(In[0]);
	if (found != SkipSet->end()) {
		for (auto it = found->second->begin();it != found->second->end();it++) {
			if ((*it).View() == In) {
				return false;
			}
		}

		found->second->push_back(in);
		return true;
	}

	auto newVec = new std::vector<CMString>();
	newVec->push_back(in);

	(*SkipSet)[In[0]] = newVec;

	return true;
}

ConfusableMatcher::ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap, std::unordered_set<std::string> Skip, bool AddDefaultValues)
{
	TheMap = new google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				CMString, // Key whole
				CMInnerHashMap*
			>*
		>*,
		HASHCOMPARE_CLS<char>,
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

	SkipSet = new google::dense_hash_map<
		char,
		std::vector<CMString>*,
		HASHCOMPARE_CLS<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<CMString>*>>
	>;
	SkipSet->set_empty_key(U'\x0');
	SkipSet->set_deleted_key(U'\x1');

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

void ConfusableMatcher::GetMappings(CMStringView Key, CMStringView Value, StackVector<std::pair<CMStringView, CMStringView>> &Storage)
{
	assert(Key.size() >= 1);
	assert(Value.size() >= 1);

	Storage.Reset();

	// Find array of whole keys
	auto keyArr = TheMap->find(Key[0]);
	if (keyArr == TheMap->end())
		return;

	for (auto it = keyArr->second->begin();it != keyArr->second->end();it++) {
		if ((*it)->first.Len <= Key.size() && ((*it)->first).View() == CMStringView(Key.data(), (*it)->first.Len)) {
			// Whole key found, search for value array
			auto foundArr = (*it)->second->find(Value[0]);
			if (foundArr == (*it)->second->end())
				continue;

			for (auto it2 = foundArr->second->begin();it2 != foundArr->second->end();it2++) {
				if (it2->Len <= Value.size() && (*it2).View() == CMStringView(Value.data(), it2->Len))
					Storage.Push(std::pair<CMStringView, CMStringView>(((*it)->first).View(), (*it2).View())); // Whole value found
			}
		}
	}

	return;
}

CM_RETURN_STATUS ConfusableMatcher::CheckWordBoundary(CMStringView In, CMStringView Match)
{
	bool startPass = false, endPass = false;

	if (In.data() == Match.data()) {
		startPass = true;
	} else {
		for (auto a = 0;a < std::extent<decltype(WORD_BOUNDARIES)>::value;a++) {
			auto wb = WORD_BOUNDARIES[a];

			if (Match.data() - wb.size() >= In.data() && CMStringView(Match.data() - wb.size(), wb.size()) == wb) {
				startPass = true;
				break;
			}
		}
	}

	if (!startPass) {
		return WORD_BOUNDARY_FAIL_START;
	}

	if (In.data() + In.size() == Match.data() + Match.size()) {
		endPass = true;
	} else {
		for (auto a = 0;a < std::extent<decltype(WORD_BOUNDARIES)>::value;a++) {
			auto wb = WORD_BOUNDARIES[a];

			if (Match.data() + Match.size() + wb.size() <= In.data() + In.size() && CMStringView(Match.data() + Match.size(), wb.size()) == WORD_BOUNDARIES[a]) {
				endPass = true;
				break;
			}
		}
	}

	if (!endPass) {
		return WORD_BOUNDARY_FAIL_END;
	}

	return MATCH;
}

CMReturn ConfusableMatcher::IndexOfInner(const CMStringView FullIn, MatchingState State, size_t *StatePushes, const CMOptions Options)
{
	std::stack<MatchingState> matchingStates;
	StackVector<std::pair<CMStringView, CMStringView>> mappingsStorage;
	CMReturn ret;
	ret.Start = State.StartingIndex;
	ret.Status = NO_MATCH;

	auto handleEmptyMatchingStates = [&State, Options, &ret, FullIn]()
	{
		if (State.Contains.size() == 0) {
			if (Options.MatchOnWordBoundary) {
				ret.Status = CheckWordBoundary(FullIn, CMStringView(FullIn.data() + State.StartingIndex, State.MatchedChars));
			} else
				ret.Status = MATCH;

			ret.Size = State.MatchedChars;
		}

		return ret;
	};

	while (true) {
		if (State.In.size() == 0) {
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
			GetMappings(State.LastMatched, State.In, mappingsStorage);
			auto mappingsSize = mappingsStorage.Size();

			if (mappingsSize != 0) {
				*StatePushes += mappingsSize;
				if (*StatePushes > Options.StatePushLimit) {
					ret.Size = State.MatchedChars;
					ret.Status = STATE_PUSH_LIMIT_EXCEEDED;
					return ret;
				}

				// Push every new matching path
				for (auto x = 0;x < mappingsStorage.CurSize;x++) {
					auto item = mappingsStorage.IsStack ? mappingsStorage.Stack[x] : mappingsStorage.Heap[x];

					matchingStates.push(MatchingState(
						CMStringView(State.In.data() + item.second.size()),
						State.Contains,
						State.StartingIndex,
						State.MatchedChars + item.second.size(),
						State.LastMatched
					));
				}
			}
		}

		// Try to skip specified substrings
		auto foundSkip = SkipSet->find(State.In[0]);
		if (foundSkip != SkipSet->end()) {
			for (auto it = foundSkip->second->begin();it != foundSkip->second->end();it++) {
				auto thisSz = it->Len;

				if (thisSz <= State.In.size() && CMStringView(State.In.data(), thisSz) == (*it).View()) {
					// Push new path

					matchingStates.push(MatchingState(
						CMStringView(State.In.data() + thisSz),
						State.Contains,
						State.StartingIndex,
						State.MatchedChars + thisSz,
						State.LastMatched
					));

					(*StatePushes)++;
					if (*StatePushes > Options.StatePushLimit) {
						ret.Size = State.MatchedChars;
						ret.Status = STATE_PUSH_LIMIT_EXCEEDED;
						return ret;
					}
				}
			}
		}

		// Try to match next substring
		size_t mappingsSize = 0;
		if (State.Contains.size() != 0) {
			GetMappings(State.Contains, State.In, mappingsStorage);
			mappingsSize = mappingsStorage.Size();
		}

		if (mappingsSize != 0) {
			*StatePushes += mappingsSize;
			if (*StatePushes > Options.StatePushLimit) {
				ret.Size = State.MatchedChars;
				ret.Status = STATE_PUSH_LIMIT_EXCEEDED;
				return ret;
			}

			for (auto x = 0;x < mappingsStorage.CurSize;x++) {
				auto item = mappingsStorage.IsStack ? mappingsStorage.Stack[x] : mappingsStorage.Heap[x];

				// If we were about to eat all of `Contains`, that means we found the whole thing
				if (item.first.size() == State.Contains.size()) {
					// But check for word boundary first
					ret.Size = State.MatchedChars + item.second.size();

					if (Options.MatchOnWordBoundary) {
						ret.Status = CheckWordBoundary(FullIn, CMStringView(FullIn.data() + State.StartingIndex, ret.Size));
					} else
						ret.Status = MATCH;

					if (ret.Status == MATCH) {
						return ret;
					} else {
						// We were about to eat the whole thing but it turned out to be not what we need, so this path is not pushed
						mappingsSize = 0;
						break;
					}
				}

				// Push new path
				matchingStates.push(MatchingState(
					CMStringView(State.In.data() + item.second.size()),
					CMStringView(State.Contains.data() + item.first.size()),
					State.StartingIndex,
					State.MatchedChars + item.second.size(),
					CMStringView(State.Contains.data(), item.first.size())
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
	}
}

CMReturn ConfusableMatcher::IndexOfFromView(CMStringView In, CMStringView Contains, CMOptions Options)
{
	StackVector<std::pair<CMStringView, CMStringView>> MappingsStorage;
	size_t statePushes = 0;
	CMReturn ret;
	ret.Start = 0;
	ret.Size = 0;
	ret.Status = NO_MATCH;

	/*
	 * No need to check for `In` size, loop never executes due it's size and returns a proper result:
	 *  In(1), Contains(0) returns a match due to check in IndexOf
	 *  In(0), Contains(1) returns no match due to return at the end of the method
	 *  In(0), Contains(0) returns a match due to check in IndexOf (nothing does contain nothing)
	 */

	for (int x = Options.StartIndex;Options.StartFromEnd ? (x >= 0) : (x < In.size());x += Options.StartFromEnd ? -1 : 1) {
		// Find the beginning and construct state from it
		GetMappings(Contains, CMStringView(In.data() + x), MappingsStorage);
		if (MappingsStorage.Size() == 0)
			continue;

		statePushes += MappingsStorage.Size();
		if (statePushes > Options.StatePushLimit) {
			ret.Start = x;
			ret.Size = 1;
			ret.Status = STATE_PUSH_LIMIT_EXCEEDED;
			return ret;
		}

		for (auto i = 0;i < MappingsStorage.CurSize;i++) {
			auto item = MappingsStorage.IsStack ? MappingsStorage.Stack[i] : MappingsStorage.Heap[i];

			// Matched full string and no repeats
			if (item.first.size() == Contains.size() && x + item.second.size() == In.size()) {
				if (Options.MatchOnWordBoundary)
					ret.Status = CheckWordBoundary(In, CMStringView(In.data() + x, item.second.size()));
				else
					ret.Status = MATCH;

				ret.Start = x;
				ret.Size = item.second.size();

				if (ret.Status == MATCH) {
					return ret;
				} else {
					assert(ret.Status != WORD_BOUNDARY_FAIL_END);
					continue;
				}
			}

			auto result = IndexOfInner(
				In,
				MatchingState(
					CMStringView(In.data() + x + item.second.size()),
					CMStringView(Contains.data() + item.first.size()),
					x,
					item.second.size(),
					CMStringView(Contains.data(), item.first.size())
				),
				&statePushes,
				Options
			);

			if (result.Status == NO_MATCH)
				continue;
			else if (result.Status == WORD_BOUNDARY_FAIL_START || result.Status == WORD_BOUNDARY_FAIL_END) {
				// Prefer word boundary failure results as they probably make more sense for human
				ret.Start = result.Start;
				ret.Size = result.Size;
				ret.Status = result.Status;
				continue;
			} else {
				// Final result - match or state push limit expired
				return result;
			}
		}
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

	if (SkipSet->size() != 0) {
		for (auto it = SkipSet->begin();it != SkipSet->end();it++) {
			for (auto it2 = it->second->begin();it2 != it->second->end();it2++) {
				it2->Free();
			}
			delete it->second;
		}
	}
	delete SkipSet;
}