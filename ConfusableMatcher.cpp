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
				auto s = valueIterator->second;

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

int ConfusableMatcher::GetRepeatingLength(CMStringView In, CMStringView FullContains, int *StatePushes, int StatePushLimit)
{
	std::stack<CMStringView> inStates;
	StackVector<std::pair<CMStringView, CMStringView>> mappingsStorage;
	const int originalInSize = In.size();
	int longestRepeatingState = 0;

	assert(In.size() > 0);
	assert(FullContains.size() > 0);

	while (true) {
		if (In.size() == 0) {
			// Ate all of `In`, so all of the string consists of repeating chars
			return originalInSize;
		}
		
		if (originalInSize - In.size() > longestRepeatingState)
			longestRepeatingState = originalInSize - In.size();

		// Try to match repeating substring
		GetMappings(FullContains, In, mappingsStorage);
		auto mappingsSize = mappingsStorage.Size();

		if (mappingsSize != 0) {
			*StatePushes += mappingsSize;
			if (*StatePushes > StatePushLimit) {
				// Best we can do irhgt now
				return longestRepeatingState;
			}

			// Push every new matching path
			for (auto x = 0;x < mappingsStorage.CurSize;x++) {
				auto item = mappingsStorage.IsStack ? mappingsStorage.Stack[x] : mappingsStorage.Heap[x];

				inStates.push(CMStringView(In.data() + item.second.size()));
			}
		} else if (inStates.size() == 0)
			return longestRepeatingState;

		In = inStates.top();
		inStates.pop();
	}
}

std::pair<int, int> ConfusableMatcher::IndexOfInner(MatchingState State, bool MatchRepeating, int *StatePushes, int StatePushLimit)
{
	std::stack<MatchingState> matchingStates;
	StackVector<std::pair<CMStringView, CMStringView>> mappingsStorage;

	assert(State.Contains.size() > 0);

	while (true) {
		if (State.In.size() == 0) {
			// Current state ate all of `In` and we have no other state to get from stack
			if (matchingStates.empty())
				return std::pair<int, int>(-1, -1);

			// This happens when there are still matches in stack but current state ate all of `In`
			State = matchingStates.top();
			matchingStates.pop();
			continue;
		}
		
		if (MatchRepeating) {
			// Try to match repeating substring
			GetMappings(State.LastMatched, State.In, mappingsStorage);
			auto mappingsSize = mappingsStorage.Size();

			if (mappingsSize != 0) {
				*StatePushes += mappingsSize;
				if (*StatePushes > StatePushLimit)
					return std::pair<int, int>(-2, -2);

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
					if (*StatePushes > StatePushLimit)
						return std::pair<int, int>(-2, -2);
				}
			}
		}

		// Try to match next substring
		GetMappings(State.Contains, State.In, mappingsStorage);
		auto mappingsSize = mappingsStorage.Size();

		if (mappingsSize != 0) {
			*StatePushes += mappingsSize;
			if (*StatePushes > StatePushLimit)
				return std::pair<int, int>(-2, -2);

			for (auto x = 0;x < mappingsStorage.CurSize;x++) {
				auto item = mappingsStorage.IsStack ? mappingsStorage.Stack[x] : mappingsStorage.Heap[x];

				// If we were about to eat all of `Contains`, that means we found the whole thing
				if (item.first.size() == State.Contains.size())
					return std::pair<int, int>(State.StartingIndex, State.MatchedChars + item.second.size());

				// Push new path
				matchingStates.push(MatchingState(
					CMStringView(State.In.data() + item.second.size()),
					CMStringView(State.Contains.data() + item.first.size()),
					State.StartingIndex,
					State.MatchedChars + item.second.size(),
					CMStringView(State.Contains.data(), item.first.size())
				));
			}
		} else {
			// We didn't put any new paths into stack so check if we depleted all paths
			if (matchingStates.empty())
				return std::pair<int, int>(-1, -1);
		}

		// Fetch newest path --- this includes last new path from next substring matching
		State = matchingStates.top();
		matchingStates.pop();
	}
}

std::pair<int, int> ConfusableMatcher::IndexOfFromView(CMStringView In, CMStringView Contains, bool MatchRepeating, int StartIndex, bool StartFromEnd, int StatePushLimit)
{
	StackVector<std::pair<CMStringView, CMStringView>> MappingsStorage;
	int statePushes = 0;

	if (Contains.size() == 0)
		return std::pair<int, int>(0, 0);

	/*
	 * No need to check for `In` size, loop never executes due it's size and returns a proper result:
	 *  In(1), Contains(0) returns (0, 0) due to check above
	 *  In(0), Contains(1) returns (-1, -1) due to return at the end of the method
	 *  In(0), Contains(0) returns (0, 0) due to check above (nothing does contain nothing)
	 */

	for (int x = StartIndex;StartFromEnd ? (x >= 0) : (x < In.size());x += StartFromEnd ? -1 : 1) {
		// Find the beginning and construct state from it
		GetMappings(Contains, CMStringView(In.data() + x), MappingsStorage);
		if (MappingsStorage.Size() == 0)
			continue;

		statePushes += MappingsStorage.Size();
		if (statePushes > StatePushLimit)
			return std::pair<int, int>(-2, -2);

		for (auto i = 0;i < MappingsStorage.CurSize;i++) {
			auto item = MappingsStorage.IsStack ? MappingsStorage.Stack[i] : MappingsStorage.Heap[i];

			// Already found the whole thing when searching for beginning
			if (item.first.size() == Contains.size()) {
				// But lets check if it repeats before returning

				auto retLength = item.second.size();

				if (MatchRepeating) {
					retLength = GetRepeatingLength(CMStringView(In.data() + x), Contains, &statePushes, StatePushLimit);
				}
				return std::pair<int, int>(x, retLength);
			}

			auto contains = IndexOfInner(MatchingState(
				CMStringView(In.data() + x + item.second.size()),
				CMStringView(Contains.data() + item.first.size()),
				x,
				item.second.size(),
				CMStringView(Contains.data(), item.first.size())
			), MatchRepeating, &statePushes, StatePushLimit);

			if (contains.first == -1)
				continue;

			return contains;
		}
	}
	return std::pair<int, int>(-1, -1);
}

std::pair<int, int> ConfusableMatcher::IndexOf(std::string In, std::string Contains, bool MatchRepeating, int StartIndex, bool StartFromEnd, int StatePushLimit)
{
	if (StartIndex >= In.size() || StartIndex < 0)
		return std::pair<int, int>(-1, -1);

	return IndexOfFromView(CMStringView(In), CMStringView(Contains), MatchRepeating, StartIndex, StartFromEnd, StatePushLimit);
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