#include <iostream>
#include <chrono>

#include "ConfusableMatcher.h"

bool ConfusableMatcher::RemoveMapping(char Key, std::string Value)
{
	dense_hash_map<char, std::vector<std::string> *> *dict;

	if ((dict = TheMap[Key]) == nullptr)
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
					TheMap.erase(Key);
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
	if (Value.length() == 1 && Key == Value[0])
		return false;

	auto dict = TheMap.find(Key);

	if (dict == TheMap.end()) {
		// Root doesn't exist
		auto newDict = new dense_hash_map<char, std::vector<std::string> *>;
		newDict->set_empty_key(U'\x0');
		newDict->set_deleted_key(U'\x1');
		TheMap[Key] = newDict;

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
	TheMap.set_empty_key(U'\x0');
	TheMap.set_deleted_key(U'\x1');

	for (auto it = InputMap.begin();it != InputMap.end();++it) {
		char k;
		std::string v;
		std::tie(k, v) = *it;

		AddMapping(k, v, true);
	}
}

std::vector<std::string> *ConfusableMatcher::GetValues(char Key, char ValFirstPart)
{
	auto dict = TheMap.find(Key);
	if (dict == TheMap.end())
		return nullptr;

	auto foundArr = dict->second->find(ValFirstPart);
	if (foundArr == dict->second->end())
		return nullptr;

	return foundArr->second;
}

int ConfusableMatcher::GetMatchedLengthSingleChar(std::string_view In, char Match)
{
	if (In[0] == Match)
		return 1;
	auto vals = GetValues(Match, In[0]);

	if (vals == nullptr)
		return -1;

	for (auto it = vals->begin();it != vals->end();++it) {
		if (it->length() <= In.length() && *it == std::string_view(In.data(), it->length())) {
			return it->length();
		}
	}

	return -1;
}
	
std::tuple<int, int> ConfusableMatcher::StringContains(std::string In, std::string Contains, std::unordered_set<char> SkipChars, bool MatchRepeating, int StartIndex)
{
	auto valIndex = StartIndex;
	char32_t lastMatchedChar = U'\x0';

	while (valIndex < In.length()) {
		auto sIndex = INT32_MAX;
		auto matched = 0;

		for (auto x = valIndex;x < In.length();x++) {
			matched = GetMatchedLengthSingleChar(std::string_view(In.data() + x, In.length() - x), Contains[0]);
			if (matched > 0) {
				sIndex = x + matched;
				lastMatchedChar = Contains[0];
				break;
			}
		}

		if (sIndex == INT32_MAX)
			return std::make_tuple(-1, -1);

		valIndex = sIndex;
		auto indexMarker = valIndex - matched;
		size_t containsMatched = 1;

		auto matchedSameInARow = 0;
		while (true) {
			auto methodPositive = false;

			if (MatchRepeating) {
				// Try to match repeating char
				matched = GetMatchedLengthSingleChar(std::string_view(In.data() + valIndex, In.length() - valIndex), lastMatchedChar);
				if (matched > 0)
					methodPositive = true;
			}
			if (methodPositive) {
				matchedSameInARow++;
				valIndex += matched;
				continue;
			}
			// Try to skip chars
			while (true) {
				if (!SkipChars.contains(In[valIndex]))
					break;
				valIndex++;
				methodPositive = true;
			}
			if (methodPositive)
				continue;

			if (matchedSameInARow > 0) {
				auto inContains = 0;
				while (Contains[containsMatched + inContains - 1] == lastMatchedChar)
					inContains++;
				matchedSameInARow = std::min(matchedSameInARow, inContains - 1);
				containsMatched += matchedSameInARow;
				matchedSameInARow = 0;
			}

			// Try to match next char
			matched = GetMatchedLengthSingleChar(std::string_view(In.data() + valIndex, In.length() - valIndex), Contains[containsMatched]);
			if (matched < 0) {
				break;
			} else {
				lastMatchedChar = Contains[containsMatched];
			}
			valIndex += matched;
			containsMatched++;

			if (containsMatched == Contains.length())
				return std::make_tuple(indexMarker, valIndex - indexMarker);
		}
	}
	return std::make_tuple(-1, -1);
}

ConfusableMatcher::~ConfusableMatcher()
{
	if (TheMap.size() == 0)
		return;
	for (auto it = TheMap.begin();it != TheMap.end(); ++it) {
		for (auto it2 = it->second->begin();it2 != it->second->end(); ++it2) {
			delete it2->second;
		}
		delete it->second;
	}
	TheMap.clear();
}