#include <iostream>
#include <chrono>
#include <stack>
#include <sstream>

#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
#include "ConfusableMatcher.h"

bool ConfusableMatcher::RemoveMapping(std::string Key, std::string Value)
{
	std::vector<
		std::pair<
			std::string, // Key whole
			google::dense_hash_map<
				char, // Value first char
				std::vector<std::string>* // Values whole
			>*
		>*
	> *keyArr;
	auto findKeyArr = TheMap->find(Key[0]);

	if (findKeyArr == TheMap->end())
		return false;
	keyArr = findKeyArr->second;

	google::dense_hash_map<
		char, // Value first char
		std::vector<std::string>* // Values whole
	> *valDict = nullptr;
	
	// Search for whole matching key
	for (auto it = keyArr->begin();it != keyArr->end();it++) {
		if ((*it)->first == Key) {
			valDict = (*it)->second;
			break;
		}
	}

	if (valDict == nullptr)
		return false;

	// Find values
	std::vector<std::string> *valArr;
	auto findValArr = valDict->find(Value[0]);
	if (findValArr == valDict->end())
		return false;
	valArr = findValArr->second;
	
	for (auto it = valArr->begin();it != valArr->end();) {
		if (*it != Value) {
			// Search for whole matching value
			it++;
			continue;
		}

		it = valArr->erase(it);

		if (valArr->size() != 0)
			return true;

		// Free value array if it's empty
		valDict->erase(Value[0]);
		delete valArr;

		if (valDict->size() != 0)
			return true;

		// Free value chars dictionary if it's empty
		delete valDict;

		bool erased = false;
		for (auto it2 = keyArr->begin();it2 != keyArr->end();) {
			if ((*it2)->first == Key) {
				auto oldPair = *it2;
				it2 = keyArr->erase(it2);
				delete oldPair; // Free the pair that was holding value chars dictionary too
				erased = true;
			} else
				it2++;
		}

		assert(erased);
		
		if (keyArr->size() != 0)
			return true;

		// Free key array if it's empty
		TheMap->erase(Key[0]);
		delete keyArr;

		return true;
	}

	return false;
}

bool ConfusableMatcher::AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate)
{
	if (Key.length() == 0)
		throw std::exception("Key.length() == 0");
	if (Value.length() == 0)
		throw std::exception("Value.length() == 0");
	if (Key[0] == '\x00' || Key[0] == '\x01')
		throw std::exception("Key cannot begin with \x00 or \x01");
	if (Value[0] == '\x00' || Value[0] == '\x01')
		throw std::exception("Value cannot begin with \x00 or \x01");

	std::vector<
		std::pair<
			std::string, // Key whole
			google::dense_hash_map<
				char, // Value first char
				std::vector<std::string>* // Values whole
			>*
		>*
	>* keyArr;
	auto findKeyArr = TheMap->find(Key[0]);

	if (findKeyArr == TheMap->end()) {
		// Root doesn't exist
		keyArr = new std::vector<
			std::pair<
				std::string, // Key whole
				google::dense_hash_map<
					char, // Value first char
					std::vector<std::string>* // Values whole
				>*
			>*
		>;
		(*TheMap)[Key[0]] = keyArr;
	} else
		keyArr = findKeyArr->second;

	google::dense_hash_map<
		char, // Value first char
		std::vector<std::string>* // Values whole
	> *valDict = nullptr;

	for (auto it = keyArr->begin();it != keyArr->end();it++) {
		if ((*it)->first == Key) {
			valDict = (*it)->second;
			break;
		}
	}

	if (valDict == nullptr) {
		// Value dictionary doesn't exist
		valDict = new google::dense_hash_map<
			char, // Value first char
			std::vector<std::string> * // Values whole
		>;
		valDict->set_empty_key('\x0');
		valDict->set_deleted_key('\x1');
		keyArr->push_back(new std::pair(Key, valDict));
	}

	std::vector<std::string> *valArr;
	auto findValArr = valDict->find(Value[0]);
	if (findValArr == valDict->end()) {
		// Value array doesn't exist
		valArr = new std::vector<std::string>;
		(*valDict)[Value[0]] = valArr;
	} else
		valArr = findValArr->second;

	if (CheckValueDuplicate) {
		for (auto it = valArr->begin();it != valArr->end();it++) {
			if (*it == Value) {
				return false;
			}
		}
	}

	valArr->push_back(Value);
	return true;
}

ConfusableMatcher::ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap, bool AddDefaultValues)
{
	TheMap = new google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				std::string, // Key whole
				google::dense_hash_map<
					char, // Value first char
					std::vector<std::string>* // Values whole
				>*
			>*
		>*
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
}

void ConfusableMatcher::GetMappings(CMStringView Key, CMStringView Value, StackVector<std::pair<std::string, std::string>> &Storage)
{
	assert(Key.length() >= 1);
	assert(Value.length() >= 1);

	Storage.Reset();

	// Find array of whole keys
	auto keyArr = TheMap->find(Key[0]);
	if (keyArr == TheMap->end())
		return;

	for (auto it = keyArr->second->begin();it != keyArr->second->end();it++) {
		if ((*it)->first.length() <= Key.length() && (*it)->first == CMStringView(Key.data(), (*it)->first.length())) {
			// Whole key found, search for value array
			auto foundArr = (*it)->second->find(Value[0]);
			if (foundArr == (*it)->second->end())
				continue;

			for (auto it2 = foundArr->second->begin();it2 != foundArr->second->end();it2++) {
				if (it2->length() <= Value.length() && *it2 == CMStringView(Value.data(), it2->length()))
					Storage.Push(std::pair((*it)->first, *it2)); // Whole value found
			}
		}
	}

	return;
}

std::pair<int, int> ConfusableMatcher::IndexOfInner(MatchingState State, std::unordered_set<std::string> Skip, bool MatchRepeating)
{
	std::stack<MatchingState> matchingStack;
	StackVector<std::pair<std::string, std::string>> mappingsStorage;

	assert(State.Contains.length() > 0);

	while (true) {
		if (State.In.length() == 0) {
			// Current state ate all of `In` and we have no other state to get from stack
			if (matchingStack.empty())
				return std::pair(-1, -1);

			// This happens when there are still matches in stack but current state ate all of `In`
			State = matchingStack.top();
			matchingStack.pop();
			continue;
		}
		
		if (MatchRepeating) {
			// Try to match repeating substring
			GetMappings(State.LastMatched, State.In, mappingsStorage);
			if (mappingsStorage.Size() != 0) {
				// Push every new matching path
				if (mappingsStorage.IsStack) {
					for (auto x = 0;x < mappingsStorage.CurSize;x++) {
						matchingStack.push(MatchingState(
							CMStringView(State.In.data() + mappingsStorage.Stack[x].second.length()),
							State.Contains,
							State.StartingIndex,
							State.MatchedChars + mappingsStorage.Stack[x].second.length(),
							State.LastMatched
						));
					}
				} else {
					// Heap ver., code kinda repeats but I'm not dealing with custom iterators
					for (auto it = mappingsStorage.Heap.begin();it != mappingsStorage.Heap.end();it++) {
						matchingStack.push(MatchingState(
							CMStringView(State.In.data() + it->second.length()),
							State.Contains,
							State.StartingIndex,
							State.MatchedChars + it->second.length(),
							State.LastMatched
						));
					}
				}
			}
		}

		// Try to skip specified substrings
		int skipBytes = 0;
		bool skippedAny;
		do {
			skippedAny = false;
			for (auto it = Skip.begin();it != Skip.end();it++) {
				if (it->size() <= (State.In.size() - skipBytes) && CMStringView(State.In.data() + skipBytes, it->size()) == *it) {
					skipBytes += it->size();
					skippedAny = true;
				}
			}
		} while (skippedAny);

		if (skipBytes != 0) {
			// Push new path
			matchingStack.push(MatchingState(
				CMStringView(State.In.data() + skipBytes),
				State.Contains,
				State.StartingIndex,
				State.MatchedChars + skipBytes,
				State.LastMatched
			));
		}

		// Try to match next substring
		GetMappings(State.Contains, State.In, mappingsStorage);
		if (mappingsStorage.Size() != 0) {
			if (mappingsStorage.IsStack) {
				for (auto x = 0;x < mappingsStorage.CurSize;x++) {
					// If we were about to eat all of `Contains`, that means we found the whole thing
					if (mappingsStorage.Stack[x].first.length() == State.Contains.length())
						return std::pair(State.StartingIndex, State.MatchedChars + mappingsStorage.Stack[x].second.length());

					// Push new path
					matchingStack.push(MatchingState(
						CMStringView(State.In.data() + mappingsStorage.Stack[x].second.length()),
						CMStringView(State.Contains.data() + mappingsStorage.Stack[x].first.length()),
						State.StartingIndex,
						State.MatchedChars + mappingsStorage.Stack[x].second.length(),
						CMStringView(State.Contains.data(), mappingsStorage.Stack[x].first.length())
					));
				}
			} else {
				// Heap ver.
				for (auto it = mappingsStorage.Heap.begin();it != mappingsStorage.Heap.end();it++) {
					if (it->first.length() == State.Contains.length())
						return std::pair(State.StartingIndex, State.MatchedChars + it->second.length());

					matchingStack.push(MatchingState(
						CMStringView(State.In.data() + it->second.length()),
						CMStringView(State.Contains.data() + it->first.length()),
						State.StartingIndex,
						State.MatchedChars + it->second.length(),
						CMStringView(State.Contains.data(), it->first.length())
					));
				}
			}
		} else {
			// We didn't put any new paths into stack so check if we depleted all paths
			if (matchingStack.empty())
				return std::pair(-1, -1);
		}

		// Fetch newest path --- this includes last new path from next substring matching
		State = matchingStack.top();
		matchingStack.pop();
	}
}

std::pair<int, int> ConfusableMatcher::IndexOfFromView(CMStringView In, CMStringView Contains, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex)
{
	StackVector<std::pair<std::string, std::string>> MappingsStorage;

	if (Contains.length() == 0)
		return std::pair(0, 0);

	/*
	 * No need to check for `In` length, loop never executes due it's length and returns a proper result:
	 *  In(1), Contains(0) returns (0, 0) due to check above
	 *  In(0), Contains(1) returns (-1, -1) due to return at the end of the method
	 *  In(0), Contains(0) returns (0, 0) due to check above (nothing does contain nothing)
	 */

	for (auto x = StartIndex;x < In.length();x++) {
		// Find the beginning and construct state from it
		GetMappings(Contains, CMStringView(In.data() + x), MappingsStorage);
		if (MappingsStorage.Size() == 0)
			continue;

		if (MappingsStorage.IsStack) {
			for (auto i = 0;i < MappingsStorage.CurSize;i++) {
				// Already found the whole thing when searching for beginning
				if (MappingsStorage.Stack[i].first.length() == Contains.length())
					return std::pair(x, MappingsStorage.Stack[i].second.length());

				auto contains = IndexOfInner(MatchingState(
					CMStringView(In.data() + x + MappingsStorage.Stack[i].second.length()),
					CMStringView(Contains.data() + MappingsStorage.Stack[i].first.length()),
					x,
					MappingsStorage.Stack[i].second.length(),
					CMStringView(Contains.data(), MappingsStorage.Stack[i].first.length())
				), Skip, MatchRepeating);

				if (contains.first == -1)
					continue;

				return contains;
			}
		} else {
			// Heap ver.
			for (auto it = MappingsStorage.Heap.begin();it != MappingsStorage.Heap.end();it++) {
				if (it->first.length() == Contains.length())
					return std::pair(x, it->second.length());

				auto contains = IndexOfInner(MatchingState(
					CMStringView(In.data() + x + it->second.length()),
					CMStringView(Contains.data() + it->first.length()),
					x,
					it->second.length(),
					CMStringView(Contains.data(), it->first.length())
				), Skip, MatchRepeating);

				if (contains.first == -1)
					continue;

				return contains;
			}
		}
	}
	return std::pair(-1, -1);
}

std::pair<int, int> ConfusableMatcher::IndexOf(std::string In, std::string Contains, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex)
{
	assert(StartIndex <= In.length() && StartIndex >= 0);
	return IndexOfFromView(CMStringView(In), CMStringView(Contains), Skip, MatchRepeating, StartIndex);
}

ConfusableMatcher::~ConfusableMatcher()
{
	if (TheMap->size() != 0) {
		for (auto it = TheMap->begin();it != TheMap->end();it++) {
			for (auto it2 = it->second->begin();it2 != it->second->end();it2++) {
				for (auto it3 = (*it2)->second->begin();it3 != (*it2)->second->end();it3++) {
					delete it3->second;
				}
				delete (*it2)->second;
			}
			delete it->second;
		}
	}
	delete TheMap;
}