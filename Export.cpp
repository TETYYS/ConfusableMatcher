#include "Export.h"
#include "ConfusableMatcher.h"

#include <vector>
#include <string>
#include <iostream>

CMHandle InitConfusableMatcher(CMMap Map, bool AddDefaultValues)
{
	std::vector<std::pair<std::string, std::string>> map;

	for (auto x = 0;x < Map.Size;x++) {
		auto kv = Map.Kv[x];
		map.push_back(std::pair<std::string, std::string>(std::string(kv.Key), std::string(kv.Value)));
	}

	auto matcher = new ConfusableMatcher(map, AddDefaultValues);
	return matcher;
}

void FreeConfusableMatcher(CMHandle In)
{
	delete (ConfusableMatcher*)In;
}

CMListHandle ConstructIgnoreList(char **List, int Count)
{
	auto ignore = new std::unordered_set<std::string>;
	for (auto x = 0;x < Count;x++) {
		ignore->insert(std::string(List[x]));
	}
	return ignore;
}

void FreeIgnoreList(CMListHandle List)
{
	delete (std::unordered_set<std::string>*)List;
}

uint64_t StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, CMListHandle IgnoreList)
{
	auto ignore = (std::unordered_set<std::string>*)IgnoreList;

	int ret[2];
	auto rawRet = ((ConfusableMatcher*)CM)->IndexOf(
		In,
		Contains,
		*ignore,
		MatchRepeating,
		StartIndex);
	ret[0] = rawRet.first;
	ret[1] = rawRet.second;

	return ret[0] | ((uint64_t)ret[1] << 32);
}

MAPPING_RESPONSE AddMapping(CMHandle CM, char *Key, char *Value, bool CheckValueDuplicate)
{
	try {
		return ((ConfusableMatcher *)CM)->AddMapping(Key, Value, CheckValueDuplicate) ? SUCCESS : ALREADY_EXISTS;
	} catch (std::runtime_error e) {
		if (e.what()[0] == 'K') {
			return e.what()[3] == ' ' ? INVALID_KEY : EMPTY_KEY;
		} else {
			return e.what()[5] == ' ' ? INVALID_VALUE : EMPTY_VALUE;
		}
	}
}

bool RemoveMapping(CMHandle CM, char *Key, char *Value)
{
	return ((ConfusableMatcher *)CM)->RemoveMapping(Key, Value);
}