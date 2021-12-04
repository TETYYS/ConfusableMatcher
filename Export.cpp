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

CMReturn StringIndexOf(CMHandle CM, char *In, char *Contains, CMOptions Options)
{
	return ((ConfusableMatcher*)CM)->IndexOf(
		In,
		Contains,
		Options);
}

CMReturn StringIndexOfDebugFailures(CMHandle CM, char* In, char* Contains, CMOptions Options, CMDebugFailures *DebugOut)
{
	std::vector<CMDebugFailure> failures;

	auto ret = ((ConfusableMatcher*)CM)->IndexOfDebugFailures(
		In,
		Contains,
		Options,
		&failures);

	DebugOut->Size = failures.size();
	DebugOut->Failures = new CMDebugFailure[failures.size()];

	for (auto x = 0; x < failures.size(); x++) {
		DebugOut->Failures[x] = failures[x];
	}

	return ret;
}

void FreeDebugFailures(CMDebugFailures* DebugFailures)
{
	delete DebugFailures->Failures;
}

uint32_t GetKeyMappings(CMHandle CM, char *In, const char **Output, uint32_t OutputSize)
{
	auto cm = (ConfusableMatcher*)CM;

	StackVector<CMString> output;
	cm->GetKeyMappings(In, output);

	auto outputSize = output.Size();

	if (OutputSize < outputSize)
		return outputSize;

	for (auto x = 0;x < output.Size();x++) {
		Output[x] = output.GetElement(x).Str;
	}

	return outputSize;
}

CMComputedStringPosPointersHandle ComputeStringPosPointers(CMHandle CM, char *Contains)
{
	auto cm = (ConfusableMatcher*)CM;

	return cm->ComputeStringPosPointers(std::string(Contains));
}

void FreeStringPosPointers(CMComputedStringPosPointersHandle StringPosPointers)
{
	delete (CMStringPosPointers*)StringPosPointers;
}