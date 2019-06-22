#include <iostream>
#include <chrono>
#include <stack>

#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
#include "ConfusableMatcher.h"


bool ConfusableMatcher::RemoveMapping(char Key, std::string Value)
{
	dense_hash_map<char, std::vector<std::string> *> *dict;

	if ((dict = (*TheMap)[Key]) == nullptr)
		return false;

	std::vector<std::string> *foundArr;

	if ((foundArr = (*dict)[Value[0]]) == nullptr)
		return false;
	
	for (auto it = foundArr->begin();it != foundArr->end(); ++it) {
		if (*it == Value) {
			foundArr->erase(it);

			if (foundArr->size() == 0) {
				(*dict).erase(Value[0]);
				delete foundArr;
				if ((*dict).size() == 0) {
					TheMap->erase(Key);
					delete dict;
				}
			}

			return true;
		}
	}

	return false;
}

bool ConfusableMatcher::AddMapping(char Key, std::string Value, bool CheckValueDuplicate)
{
	auto dict = TheMap->find(Key);

	if (dict == TheMap->end()) {
		// Root doesn't exist
		auto newDict = new dense_hash_map<char, std::vector<std::string> *>;
		newDict->set_empty_key(U'\x0');
		newDict->set_deleted_key(U'\x1');
		(*TheMap)[Key] = newDict;

		auto newArr = new std::vector<std::string>;
		newArr->push_back(Value);
		(*newDict)[Value[0]] = newArr;
		return true;
	} else {
		auto foundArr = dict->second->find(Value[0]);
		if (foundArr != dict->second->end()) {
			if (CheckValueDuplicate) {
				for (auto it = foundArr->second->begin();it != foundArr->second->end(); ++it) {
					if (*it == Value) {
						return false;
					}
				}
			}

			foundArr->second->push_back(Value);
			return true;
		}

		auto newArr = new std::vector<std::string>;
		newArr->push_back(Value);
		(*dict->second)[Value[0]] = newArr;
		return true;
	}
}

ConfusableMatcher::ConfusableMatcher(std::vector<std::tuple<char, std::string>> InputMap)
{
	TheMap = new google::dense_hash_map<char, google::dense_hash_map<char, std::vector<std::string>*>*>();
	TheMap->set_empty_key(U'\x0');
	TheMap->set_deleted_key(U'\x1');

	for (auto x = 'A';x <= 'Z';x++)
		AddMapping((char)x, std::string(1, (char)x), true);

	for (auto it = InputMap.begin();it != InputMap.end();++it) {
		char k;
		std::string v;
		std::tie(k, v) = *it;

		AddMapping(k, v, true);
	}
}

std::vector<std::string> *ConfusableMatcher::GetValues(char Key, char ValFirstPart)
{
	auto dict = TheMap->find(Key);
	if (dict == TheMap->end())
		return nullptr;

	auto foundArr = dict->second->find(ValFirstPart);
	if (foundArr == dict->second->end())
		return nullptr;

	return foundArr->second;
}

std::vector<int> ConfusableMatcher::GetMatchedLengthsSingleChar(std::string_view In, char Match)
{
	auto vals = GetValues(Match, In[0]);
	std::vector<int> ret;

	if (vals == nullptr) {
		ret.push_back(-1);
		return ret;
	}

	for (auto it = vals->begin();it != vals->end();++it) {
		if (it->length() <= In.length() && *it == std::string_view(In.data(), it->length())) {
			ret.push_back(it->length());
		}
	}

	if (ret.size() == 0)
		ret.push_back(-1);

	return ret;
}

std::tuple<int, int> ConfusableMatcher::StringContainsInner(MatchingState State, MATCHING_MODE Mode, std::unordered_set<char> SkipChars, bool MatchRepeating)
{
	std::stack<MatchingState> MatchingStack;

	if (State.In.length() == 0)
		return std::make_tuple(-1, -1);

	// Input string contains a 0 width pixel space for sure :)
	if (State.Contains.length() == 0)
		return std::make_tuple(0, 0);

	while (true) {
		if (MatchRepeating) {
			// Try to match repeating char
			auto matchedRepeating = GetMatchedLengthsSingleChar(State.In, State.LastMatchedChar);
			if (matchedRepeating[0] != -1) {
				// Push every new matching path
				for (auto it = matchedRepeating.begin();it != matchedRepeating.end();++it) {
					MatchingStack.push(MatchingState(
						std::string_view(State.In.data() + *it),
						State.Contains,
						State.StartingIndex,
						State.MatchedChars + *it,
						State.LastMatchedChar
					));
				}
			}
		}

		// Try to skip chars
		int skipCount;
		for (skipCount = 0;;skipCount++) {
			if (!SkipChars.contains(State.In[skipCount]))
				break;
		}

		// Push new path if applicable
		if (skipCount != 0) {
			MatchingStack.push(MatchingState(
				std::string_view(State.In.data() + skipCount),
				State.Contains,
				State.StartingIndex,
				State.MatchedChars + skipCount,
				State.LastMatchedChar
			));
		}

		// Try to match next char - main technique
		auto matchedNext = GetMatchedLengthsSingleChar(State.In, State.Contains[0]);
		if (matchedNext[0] != -1) {
			if (State.Contains.length() <= 1) {
				// If we have only 1 `contains` char right now, return entirely as we matched the entire string
				return std::make_tuple(State.StartingIndex, State.MatchedChars + matchedNext[0]);
			} else {
				// Else, drop one char from `contains`, as all matches match only one char
				State.LastMatchedChar = State.Contains[0];
				State.Contains = std::string_view(State.Contains.data() + 1);
			}

			for (auto it = matchedNext.begin();it != matchedNext.end();++it) {
				MatchingStack.push(MatchingState(
					std::string_view(State.In.data() + *it),
					State.Contains, // We already dropped on char from this
					State.StartingIndex,
					State.MatchedChars + *it,
					State.LastMatchedChar // This one is already set earlier
				));
			}
		} else {
			if (MatchingStack.empty())
				return std::make_tuple(-1, -1);
		}
		State = MatchingStack.top();
		MatchingStack.pop();
	}
}

std::tuple<int, int> ConfusableMatcher::StringContainsFromView(std::string_view In, std::string_view Contains, MATCHING_MODE Mode, std::unordered_set<char> SkipChars, bool MatchRepeating, int StartIndex)
{
	for (auto x = StartIndex;x < In.length();x++) {
		auto matched = GetMatchedLengthsSingleChar(std::string_view(In.data() + x), Contains[0]);
		if (matched.size() == 1 && matched[0] == -1)
			continue;

		for (auto it = matched.begin();it != matched.end();++it) {
			auto contains = StringContainsInner(MatchingState(
				std::string_view(In.data() + x + *it),
				Contains.data() + 1,
				x,
				*it,
				Contains[0]
			), Mode, SkipChars, MatchRepeating);

			if (std::get<0>(contains) == -1)
				continue;

			return contains;
		}
	}
	return std::make_tuple(-1, -1);
}

std::tuple<int, int> ConfusableMatcher::StringContains(std::string In, std::string Contains, MATCHING_MODE Mode, std::unordered_set<char> SkipChars, bool MatchRepeating, int StartIndex)
{
	assert(StartIndex <= In.length() && StartIndex >= 0);
	return StringContainsFromView(std::string_view(In), std::string_view(Contains), Mode, SkipChars, MatchRepeating, StartIndex);
}

ConfusableMatcher::~ConfusableMatcher()
{
	if (TheMap->size() != 0) {
		for (auto it = TheMap->begin();it != TheMap->end(); ++it) {
			for (auto it2 = it->second->begin();it2 != it->second->end(); ++it2) {
				delete it2->second;
			}
			delete it->second;
		}
	}
	delete TheMap;
}