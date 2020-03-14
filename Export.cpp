#include "Export.h"
#include "ConfusableMatcher.h"

#include <vector>
#include <string>
#include <iostream>

CMHandle InitConfusableMatcher(CMMap Map, char** IgnoreList, int IgnoreCount, bool AddDefaultValues)
{
	std::vector<std::pair<std::string, std::string>> map;

	for (auto x = 0;x < Map.Size;x++) {
		auto kv = Map.Kv[x];
		map.push_back(std::pair<std::string, std::string>(std::string(kv.Key), std::string(kv.Value)));
	}

	std::unordered_set<std::string> ignore;
	for (auto x = 0; x < IgnoreCount; x++) {
		ignore.insert(std::string(IgnoreList[x]));
	}

	auto matcher = new ConfusableMatcher(map, ignore, AddDefaultValues);
	return matcher;
}

void FreeConfusableMatcher(CMHandle In)
{
	delete (ConfusableMatcher*)In;
}

uint64_t StringIndexOf(CMHandle CM, char *In, char *Contains, bool MatchRepeating, int StartIndex, int StatePushLimit)
{
	int ret[2];
	auto rawRet = ((ConfusableMatcher*)CM)->IndexOf(
		In,
		Contains,
		MatchRepeating,
		StartIndex,
		StatePushLimit);
	ret[0] = rawRet.first;
	ret[1] = rawRet.second;

	return ret[0] | ((uint64_t)ret[1] << 32);
}