#include <iostream>
#include <chrono>
#include <stack>
#include <sstream>

#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
#include "ConfusableMatcher.h"
#include <Windows.h>


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
	
	for (auto it = keyArr->begin();it != keyArr->end();++it) {
		if ((*it)->first == Key) {
			valDict = (*it)->second;
			break;
		}
	}

	if (valDict == nullptr)
		return false;

	std::vector<std::string> *valArr;
	auto findValArr = valDict->find(Value[0]);
	if (findValArr == valDict->end())
		return false;
	valArr = findValArr->second;
	
	for (auto it = valArr->begin();it != valArr->end(); ++it) {
		if (*it != Value)
			continue;

		valArr->erase(it);

		if (valArr->size() != 0)
			return true;

		valDict->erase(Value[0]);
		delete valArr;

		if (valDict->size() != 0)
			return true;

		for (auto it2 = keyArr->begin();it2 != keyArr->end();++it2) {
			if ((*it2)->first == Key) {
				keyArr->erase(it2);
				delete *it2;
			}
		}
			
		if (keyArr->size() != 0)
			return true;

		TheMap->erase(Key[0]);
		delete keyArr;

		return true;
	}

	return false;
}

bool ConfusableMatcher::AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate)
{
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

	for (auto it = keyArr->begin();it != keyArr->end();++it) {
		if ((*it)->first == Key) {
			valDict = (*it)->second;
			break;
		}
	}

	if (valDict == nullptr) {
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
		valArr = new std::vector<std::string>;
		(*valDict)[Value[0]] = valArr;
	} else
		valArr = findValArr->second;

	if (CheckValueDuplicate) {
		for (auto it = valArr->begin();it != valArr->end(); ++it) {
			if (*it == Value) {
				return false;
			}
		}
	}

	valArr->push_back(Value);
	return true;
}

ConfusableMatcher::ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap)
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

	for (auto x = 'A';x <= 'Z';x++) {
		AddMapping(std::string(1, (char)x), std::string(1, (char)x), true);
		AddMapping(std::string(1, (char)x), std::string(1, (char)(x + 0x20)), true);
	}
	for (auto x = '0';x <= '9';x++)
		AddMapping(std::string(1, (char)x), std::string(1, (char)x), true);

	for (auto it = InputMap.begin();it != InputMap.end();++it)
		AddMapping(it->first, it->second, true);
}

void ConfusableMatcher::GetMappings(std::string_view Key, std::string_view Value, StackVector<std::pair<std::string, std::string>> &Storage)
{
	assert(Key.length() >= 1);
	assert(Value.length() >= 1);

	Storage.Reset();

	auto keyArr = TheMap->find(Key[0]);
	if (keyArr == TheMap->end())
		return;

	for (auto it = keyArr->second->begin();it != keyArr->second->end();++it) {
		if ((*it)->first.length() <= Key.length() && (*it)->first == std::string_view(Key.data(), (*it)->first.length())) {
			auto foundArr = (*it)->second->find(Value[0]);
			if (foundArr == (*it)->second->end())
				continue;

			for (auto it2 = foundArr->second->begin();it2 != foundArr->second->end();++it2) {
				if (it2->length() <= Value.length() && *it2 == std::string_view(Value.data(), it2->length()))
					Storage.Push(std::pair((*it)->first, *it2));
			}
		}
	}

	return;
}

std::pair<int, int> ConfusableMatcher::IndexOfInner(MatchingState State, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating)
{
	std::stack<MatchingState> MatchingStack;
	StackVector<std::pair<std::string, std::string>> MappingsStorage;

	if (State.In.length() == 0)
		return std::pair(-1, -1);

	// Input string contains a 0 width pixel space for sure :)
	if (State.Contains.length() == 0)
		return std::pair(0, 0);

	while (true) {
		if (MatchRepeating) {
			// Try to match repeating substring
			GetMappings(State.LastMatched, State.In, MappingsStorage);
			if (MappingsStorage.Size() != 0) {
				// Push every new matching path
				if (MappingsStorage.IsStack) {
					for (auto x = 0;x < MappingsStorage.CurSize;x++) {
						MatchingStack.push(MatchingState(
							std::string_view(State.In.data() + MappingsStorage.Stack[x].second.length()),
							State.Contains,
							State.StartingIndex,
							State.MatchedChars + MappingsStorage.Stack[x].second.length(),
							State.LastMatched
						));
					}
				} else {
					for (auto it = MappingsStorage.Heap.begin();it != MappingsStorage.Heap.end();++it) {
						MatchingStack.push(MatchingState(
							std::string_view(State.In.data() + it->second.length()),
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
			for (auto it = Skip.begin();it != Skip.end();++it) {
				if (it->size() <= (State.In.size() - skipBytes) && std::string_view(State.In.data() + skipBytes, it->size()) == *it) {
					skipBytes += it->size();
					skippedAny = true;
				}
			}
		} while (skippedAny);

		// Push new path if applicable
		if (skipBytes != 0) {
			MatchingStack.push(MatchingState(
				std::string_view(State.In.data() + skipBytes),
				State.Contains,
				State.StartingIndex,
				State.MatchedChars + skipBytes,
				State.LastMatched
			));
		}

		// Try to match next char - main technique
		GetMappings(State.Contains, State.In, MappingsStorage);
		if (MappingsStorage.Size() != 0) {
			if (MappingsStorage.IsStack) {
				for (auto x = 0;x < MappingsStorage.CurSize;x++) {
					if (MappingsStorage.Stack[x].first.length() == State.Contains.length())
						return std::pair(State.StartingIndex, State.MatchedChars + MappingsStorage.Stack[x].second.length());

					MatchingStack.push(MatchingState(
						std::string_view(State.In.data() + MappingsStorage.Stack[x].second.length()),
						std::string_view(State.Contains.data() + MappingsStorage.Stack[x].first.length()),
						State.StartingIndex,
						State.MatchedChars + MappingsStorage.Stack[x].second.length(),
						std::string_view(State.Contains.data(), MappingsStorage.Stack[x].first.length())
					));
				}
			} else {
				for (auto it = MappingsStorage.Heap.begin();it != MappingsStorage.Heap.end();++it) {
					if (it->first.length() == State.Contains.length())
						return std::pair(State.StartingIndex, State.MatchedChars + it->second.length());

					MatchingStack.push(MatchingState(
						std::string_view(State.In.data() + it->second.length()),
						std::string_view(State.Contains.data() + it->first.length()),
						State.StartingIndex,
						State.MatchedChars + it->second.length(),
						std::string_view(State.Contains.data(), it->first.length())
					));
				}
			}
		} else {
			if (MatchingStack.empty())
				return std::pair(-1, -1);
		}
		State = MatchingStack.top();
		MatchingStack.pop();
	}
}

std::pair<int, int> ConfusableMatcher::IndexOfFromView(std::string_view In, std::string_view Contains, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex)
{
	StackVector<std::pair<std::string, std::string>> MappingsStorage;

	for (auto x = StartIndex;x < In.length();x++) {
		GetMappings(Contains, std::string_view(In.data() + x), MappingsStorage);
		if (MappingsStorage.Size() == 0)
			continue;

		if (MappingsStorage.IsStack) {
			for (auto i = 0;i < MappingsStorage.CurSize;i++) {
				if (MappingsStorage.Stack[i].first.length() == Contains.length())
					return std::pair(x, MappingsStorage.Stack[i].second.length());

				auto contains = IndexOfInner(MatchingState(
					std::string_view(In.data() + x + MappingsStorage.Stack[i].second.length()),
					std::string_view(Contains.data() + MappingsStorage.Stack[i].first.length()),
					x,
					MappingsStorage.Stack[i].second.length(),
					std::string_view(Contains.data(), MappingsStorage.Stack[i].first.length())
				), Mode, Skip, MatchRepeating);

				if (contains.first == -1)
					continue;

				return contains;
			}
		} else {
			for (auto it = MappingsStorage.Heap.begin();it != MappingsStorage.Heap.end();++it) {
				if (it->first.length() == Contains.length())
					return std::pair(x, it->second.length());

				auto contains = IndexOfInner(MatchingState(
					std::string_view(In.data() + x + it->second.length()),
					std::string_view(Contains.data() + it->first.length()),
					x,
					it->second.length(),
					std::string_view(Contains.data(), it->first.length())
				), Mode, Skip, MatchRepeating);

				if (contains.first == -1)
					continue;

				return contains;
			}
		}
	}
	return std::pair(-1, -1);
}

std::pair<int, int> ConfusableMatcher::IndexOf(std::string In, std::string Contains, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex)
{
	assert(StartIndex <= In.length() && StartIndex >= 0);
	return IndexOfFromView(std::string_view(In), std::string_view(Contains), Mode, Skip, MatchRepeating, StartIndex);
}

ConfusableMatcher::~ConfusableMatcher()
{
	if (TheMap->size() != 0) {
		for (auto it = TheMap->begin();it != TheMap->end(); ++it) {
			for (auto it2 = it->second->begin();it2 != it->second->end(); ++it2) {
				for (auto it3 = (*it2)->second->begin();it3 != (*it2)->second->end(); ++it3) {
					delete it3->second;
				}
				delete (*it2)->second;
			}
			delete it->second;
		}
	}
	delete TheMap;
}