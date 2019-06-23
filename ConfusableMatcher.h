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
	std::string_view LastMatched;

	MatchingState(std::string_view In, std::string_view Contains, int StartingIndex, int MatchedChars, std::string_view LastMatched)
	{
		this->In = In;
		this->Contains = Contains;
		this->StartingIndex = StartingIndex;
		this->MatchedChars = MatchedChars;
		this->LastMatched = LastMatched;
	}
};

class ConfusableMatcher
{
	private:
	google::dense_hash_map<
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
	>
		*TheMap;
	std::vector<std::pair<std::string, std::string>>
		GetMappings(std::string_view Key, std::string_view Value);
	/*std::vector<std::pair<int, int>>
		GetMatchedLengths(std::string_view In, std::string_view Contains);*/
	std::pair<int, int>
		IndexOfFromView(std::string_view In, std::string_view Contains, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex);
	std::pair<int, int>
		IndexOfInner(MatchingState State, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating);

	public:
	ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap);
	bool
		AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate);
	bool
		RemoveMapping(std::string Key, std::string Value);
	std::pair<int, int>
		IndexOf(std::string In, std::string Contains, MATCHING_MODE Mode, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex = 0);

	~ConfusableMatcher();
};