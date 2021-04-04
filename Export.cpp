#include "Export.h"
#include "ConfusableMatcher.h"

#include <vector>
#include <string>
#include <iostream>

CMHandle InitConfusableMatcher(CMMap Map, char **IgnoreList, int IgnoreCount, bool AddDefaultValues)
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

CMIntPair StringIndexOf(CMHandle CM, char *In, char *Contains, CMOptions Options)
{
	auto rawRet = ((ConfusableMatcher*)CM)->IndexOf(
		In,
		Contains,
		Options);

	return CMIntPair {
		rawRet.first, rawRet.second
	};
}

uint32_t GetKeyMappings(CMHandle CM, char *In, const char **Output, uint32_t OutputSize)
{
	auto cm = ((ConfusableMatcher*)CM);

	StackVector<CMString> output;
	cm->GetKeyMappings(In, output);

	auto outputSize = output.Size();

	if (OutputSize < outputSize)
		return outputSize;

	if (output.IsStack) {
		for (auto x = 0;x < output.CurSize;x++) {
			Output[x] = output.Stack[x].Str;
		}
	} else {
		for (auto x = 0;x < output.Heap.size();x++) {
			Output[x] = output.Heap[x].Str;
		}
	}

	return outputSize;
}