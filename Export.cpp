#include "Export.h"
#include "ConfusableMatcher.h"

#include <vector>
#include <string>
#include <iostream>

CMHandle InitConfusableMatcher(CMMap Map)
{
	std::vector<std::tuple<char32_t, std::string>> map;

	for (auto x = 0;x < Map.Size;x++) {
		auto kv = Map.Kv[x];
		map.push_back(std::make_tuple(kv.Key, std::string(kv.Value)));
	}

	auto matcher = new ConfusableMatcher(map, true);
	return matcher;
}

void FreeConfusableMatcher(CMHandle In)
{
	delete (ConfusableMatcher*)In;
}

CMListHandle ConstructIgnoreList(uint32_t *List, int Count)
{
	auto ignore = new std::unordered_set<char32_t>;
	for (auto x = 0;x < Count;x++) {
		ignore->insert(List[x]);
	}
	return ignore;
}

void FreeIgnoreList(CMListHandle List)
{
	delete (std::unordered_set<char32_t>*)List;
}

uint64_t StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, CMListHandle IgnoreList)
{
	auto ignore = (std::unordered_set<char32_t>*)IgnoreList;

	int ret[2];
	std::tie(ret[0], ret[1]) = ((ConfusableMatcher *)CM)->StringContains(
		In,
		Contains,
		*ignore,
		MatchRepeating,
		StartIndex);

	return ret[0] | ((uint64_t)ret[1] << 32);
}

bool AddMapping(CMHandle CM, uint32_t Key, char *Value, bool CheckValueDuplicate)
{
	return ((ConfusableMatcher *)CM)->AddMapping(Key, Value, CheckValueDuplicate);
}

bool RemoveMapping(CMHandle CM, uint32_t Key, char *Value)
{
	return ((ConfusableMatcher *)CM)->RemoveMapping(Key, Value);
}