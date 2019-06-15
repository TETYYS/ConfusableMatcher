#pragma once

#include <sparsehash/dense_hash_map>
#include <codecvt>
#include <string_view>
#include <unordered_set>

using google::dense_hash_map;

struct Str32
{
	std::u32string Str;

	Str32(std::u32string In);

#if _MSC_VER >= 1900
	Str32(std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> *convert, std::string &In);
#else
	Str32(std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> *convert, std::string &In);
#endif
	Str32() { }
};

class ConfusableMatcher
{
	private:
	dense_hash_map<char32_t, dense_hash_map<char32_t, std::vector<Str32> *> *>
		TheMap;

#if _MSC_VER >= 1900
	std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t>
		*Convert;
#else
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>
		*Convert;
#endif

	std::vector<Str32>
		*GetValues(char32_t Key, char32_t ValFirstPart);
	int
		GetMatchedLengthSingleChar(std::u32string_view In, char32_t Match);

	public:
	ConfusableMatcher(std::vector<std::tuple<char32_t, std::string>> InputMap, bool MakeAllUpper);
	bool
		AddMapping(char32_t Key, std::string Value, bool CheckValueDuplicate);
	bool
		RemoveMapping(char32_t Key, std::string Value);
	std::tuple<int, int>
		StringContains(std::string In_, std::string Contains_, std::unordered_set<char32_t> SkipChars, bool MatchRepeating, int StartIndex = 0);

	~ConfusableMatcher();
};