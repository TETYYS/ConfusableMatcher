#pragma once

#include <codecvt>
#include <string_view>
#include <unordered_set>

namespace google
{
	template<class T>
	class libc_allocator_with_realloc;

	template<class Key, class T, class HashFcn = stdext::hash_compare<Key>,
		class EqualKey = std::equal_to<Key>,
		class Alloc = libc_allocator_with_realloc<std::pair<const Key, T>>>
		class dense_hash_map;
};

enum MATCHING_MODE
{
	/// Match all mappings
	RECURSIVE,
	/// Match longest mapping
	LONGEST,
	/// Match shortest mapping
	SHORTEST
};

struct MatchingState
{
	std::string_view In;
	std::string_view Contains;
	int StartingIndex;
	int MatchedChars;
	char LastMatchedChar;

	MatchingState(std::string_view In, std::string_view Contains, int StartingIndex, int MatchedChars, char LastMatchedChar)
	{
		this->In = In;
		this->Contains = Contains;
		this->StartingIndex = StartingIndex;
		this->MatchedChars = MatchedChars;
		this->LastMatchedChar = LastMatchedChar;
	}
};

class ConfusableMatcher
{
	private:
	google::dense_hash_map<char, google::dense_hash_map<char, std::vector<std::string>*>*>
		*TheMap;
	std::vector<std::string>
		*GetValues(char Key, char ValFirstPart);
	std::vector<int>
		GetMatchedLengthsSingleChar(std::string_view In, char Match);
	std::tuple<int, int>
		StringContainsFromView(std::string_view In, std::string_view Contains, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex);
	std::tuple<int, int>
		StringContainsInner(MatchingState State, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating);

	public:
	ConfusableMatcher(std::vector<std::tuple<char, std::string>> InputMap);
	bool
		AddMapping(char Key, std::string Value, bool CheckValueDuplicate);
	bool
		RemoveMapping(char Key, std::string Value);
	std::tuple<int, int>
		StringContains(std::string In, std::string Contains, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex = 0);

	~ConfusableMatcher();
};