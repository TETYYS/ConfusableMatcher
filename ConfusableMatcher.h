#pragma once

#include <sparsehash/dense_hash_map>
#include <codecvt>
#include <string_view>
#include <unordered_set>

using google::dense_hash_map;

class ConfusableMatcher
{
	private:
	dense_hash_map<char, dense_hash_map<char, std::vector<std::string>*>*>
		TheMap;
	std::vector<std::string>
		*GetValues(char Key, char ValFirstPart);
	int
		GetMatchedLengthSingleChar(std::string_view In, char Match);

	public:
	ConfusableMatcher(std::vector<std::tuple<char, std::string>> InputMap, bool MakeAllUpper);
	bool
		AddMapping(char Key, std::string Value, bool CheckValueDuplicate);
	bool
		RemoveMapping(char Key, std::string Value);
	std::tuple<int, int>
		StringContains(std::string In_, std::string Contains_, std::unordered_set<char> SkipChars, bool MatchRepeating, int StartIndex = 0);

	~ConfusableMatcher();
};