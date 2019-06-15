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

ConfusableMatcher::ConfusableMatcher(std::vector<std::tuple<char, std::string>> InputMap, bool MakeAllUpper)
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

int main()
{
	std::vector<std::tuple<char, std::string>> map;

	map.push_back(std::make_tuple('N', "/[()[]]/"));
	map.push_back(std::make_tuple('N', "\U000000f1"));
	map.push_back(std::make_tuple('N', "|\\|"));
	map.push_back(std::make_tuple('N', "\U00000245\U00000002f"));
	map.push_back(std::make_tuple('N', "/IJ"));
	map.push_back(std::make_tuple('N', "/|/"));

	std::vector<std::string> ns = { "\U000004c5", "\U000003a0", "\U00000418", "\U0001d427", "\U0001d45b", "\U0001d48f", "\U0001d4c3", "\U0001d4f7", "\U0001d52b", "\U0001d55f", "\U0001d593", "\U0001d5c7", "\U0001d5fb", "\U0001d62f", "\U0001d663", "\U0001d697", "\U00000578", "\U0000057c", "\U0000ff2e", "\U00002115", "\U0001d40d", "\U0001d441", "\U0001d475", "\U0001d4a9", "\U0001d4dd", "\U0001d511", "\U0001d579", "\U0001d5ad", "\U0001d5e1", "\U0001d615", "\U0001d649", "\U0001d67d", "\U0000039d", "\U0001d6b4", "\U0001d6ee", "\U0001d728", "\U0001d762", "\U0001d79c", "\U0000a4e0", "\U00000143", "\U00000145", "\U00000147", "\U0000014b", "\U0000019d", "\U000001f8", "\U00000220", "\U0000039d", "\U00001e44", "\U00001e46", "\U00001e48", "\U00001e4a", "\U000020a6", "\U00001f20", "\U00001f21", "\U00001f22", "\U00001f23", "\U00001f24", "\U00001f25", "\U00001f26", "\U00001f27", "\U00001f74", "\U00001f75", "\U00001f90", "\U00001f91", "\U00001f92", "\U00001f93", "\U00001f94", "\U00001f95", "\U00001f96", "\U00001f97", "\U00001fc2", "\U00001fc3", "\U00001fc4", "\U00001fc6", "\U00001fc7", "\U000000f1", "\U00000144", "\U00000146", "\U00000148", "\U00000149", "\U0000014a", "\U0000019e", "\U000001f9", "\U00000235", "\U00000272", "\U00000273", "\U00000274", "\U00001d70", "\U00001d87", "\U00001e45", "\U00001e47", "\U00001e49", "\U00001e4b" };
	std::vector<std::string> is = { "\U00001ec8", "\U00000079", "\U00000069", "\U00000031", "\U0000007c", "\U0000006c", "\U0000006a", "\U00000021", "\U0000002f", "\U0000005c\U0000005c", "\U0000ff49", "\U000000a1", "\U00002170", "\U00002139", "\U00002148", "\U0001d422", "\U0001d456", "\U0001d48a", "\U0001d4be", "\U0001d4f2", "\U0001d526", "\U0001d55a", "\U0001d58e", "\U0001d5c2", "\U0001d5f6", "\U0001d62a", "\U0001d65e", "\U0001d692", "\U00000131", "\U0001d6a4", "\U0000026a", "\U00000269", "\U000003b9", "\U00001fbe", "\U0000037a", "\U0001d6ca", "\U0001d704", "\U0001d73e", "\U0001d778", "\U0001d7b2", "\U00000456", "\U000024be", "\U0000a647", "\U000004cf", "\U0000ab75", "\U000013a5", "\U00000263", "\U00001d8c", "\U0000ff59", "\U0001d432", "\U0001d466", "\U0001d49a", "\U0001d4ce", "\U0001d502", "\U0001d536", "\U0001d56a", "\U0001d59e", "\U0001d5d2", "\U0001d606", "\U0001d63a", "\U0001d66e", "\U0001d6a2", "\U0000028f", "\U00001eff", "\U0000ab5a", "\U000003b3", "\U0000213d", "\U0001d6c4", "\U0001d6fe", "\U0001d738", "\U0001d772", "\U0001d7ac", "\U00000443", "\U000004af", "\U000010e7", "\U0000ff39", "\U0001d418", "\U0001d44c", "\U0001d480", "\U0001d4b4", "\U0001d4e8", "\U0001d51c", "\U0001d550", "\U0001d584", "\U0001d5b8", "\U0001d5ec", "\U0001d620", "\U0001d654", "\U0001d688", "\U000003a5", "\U000003d2", "\U0001d6bc", "\U0001d6f6", "\U0001d730", "\U0001d76a", "\U0001d7a4", "\U00002ca8", "\U00000423", "\U000004ae", "\U000013a9", "\U000013bd", "\U0000a4ec", "\U00000176", "\U00000178", "\U000001b3", "\U00000232", "\U0000024e", "\U0000028f", "\U00001e8e", "\U00001ef2", "\U00001ef4", "\U00001ef6", "\U00001ef8", "\U0000ff39", "\U000000cc", "\U000000cd", "\U000000ce", "\U000000cf", "\U00000128", "\U0000012a", "\U0000012c", "\U0000012e", "\U00000130", "\U00000196", "\U00000197", "\U000001cf", "\U00000208", "\U0000020a", "\U0000026a", "\U0000038a", "\U00000390", "\U00000399", "\U000003aa", "\U00000406", "\U0000040d", "\U00000418", "\U00000419", "\U000004e2", "\U000004e4", "\U00001e2c", "\U00001e2e", "\U00001ec8", "\U00001eca", "\U00001fd8", "\U00001fd9", "\U00002160", "\U0000ff29", "\U000030a7", "\U000030a8", "\U0000ff6a", "\U0000ff74", "\U000000ec", "\U000000ed", "\U000000ee", "\U000000ef", "\U00000129", "\U0000012b", "\U0000012d", "\U0000012f", "\U00000131", "\U000001d0", "\U00000209", "\U0000020b", "\U00000268", "\U00000269", "\U00000365", "\U000003af", "\U000003ca", "\U00000438", "\U00000439", "\U00000456", "\U0000045d", "\U000004e3", "\U000004e5", "\U00001e2d", "\U00001e2f", "\U00001ec9", "\U00001ecb", "\U00001f30", "\U00001f31", "\U00001f32", "\U00001f33", "\U00001f34", "\U00001f35", "\U00001f36", "\U00001f37", "\U00001f76", "\U00001f77", "\U00001fbe", "\U00001fd0", "\U00001fd1", "\U00001fd2", "\U00001fd3", "\U00001fd6", "\U00001fd7", "\U0000ff49", "\U00001d85", "\U00001e37", "\U00001e39", "\U00001e3b", "\U00001e3d", "\U000000fd", "\U000000ff", "\U00000177", "\U000001b4", "\U00000233", "\U0000024f", "\U0000028e", "\U000002b8", "\U00001e8f", "\U00001e99", "\U00001ef3", "\U00001ef5", "\U00001ef7", "\U00001ef9", "\U0000ff59" };
	std::vector<std::string> gs = { "\U0000006b", "\U00000067", "\U00000071", "\U00000034", "\U00000036", "\U00000039", "\U0000011f", "\U00000d6b", "\U0000ff47", "\U0000210a", "\U0001d420", "\U0001d454", "\U0001d488", "\U0001d4f0", "\U0001d524", "\U0001d558", "\U0001d58c", "\U0001d5c0", "\U0001d5f4", "\U0001d628", "\U0001d65c", "\U0001d690", "\U00000261", "\U00001d83", "\U0000018d", "\U00000581", "\U0001d406", "\U0001d43a", "\U0001d46e", "\U0001d4a2", "\U0001d4d6", "\U0001d50a", "\U0001d53e", "\U0001d572", "\U0001d5a6", "\U0001d5da", "\U00004e48", "\U0001d60e", "\U0001d642", "\U0001d676", "\U0000050c", "\U000013c0", "\U000013f3", "\U0000a4d6", "\U0000011c", "\U0000011e", "\U00000120", "\U00000122", "\U00000193", "\U000001e4", "\U000001e6", "\U000001f4", "\U0000029b", "\U00000393", "\U00000413", "\U00001e20", "\U0000ff27", "\U000013b6", "\U0000011d", "\U0000011f", "\U00000121", "\U00000123", "\U000001e5", "\U000001e7", "\U000001f5", "\U00000260", "\U00000261", "\U00000262", "\U00000040" };
	std::vector<std::string> es = { "\U00001ec0", "\U000003a3", "\U0000039e", "\U00000065", "\U00000033", "\U00000075", "\U0000212e", "\U0000ff45", "\U0000212f", "\U00002147", "\U0001d41e", "\U0001d452", "\U0001d486", "\U0001d4ee", "\U0001d522", "\U0001d556", "\U0001d58a", "\U0001d5be", "\U0001d5f2", "\U0001d626", "\U0001d65a", "\U0001d68e", "\U0000ab32", "\U00000435", "\U000004bd", "\U000022ff", "\U0000ff25", "\U00002130", "\U0001d404", "\U0001d438", "\U0001d46c", "\U0001d4d4", "\U0001d508", "\U0001d53c", "\U0001d570", "\U0001d5a4", "\U0001d5d8", "\U0001d60c", "\U0001d640", "\U0001d674", "\U00000395", "\U0001d6ac", "\U0001d6e6", "\U0001d720", "\U0001d75a", "\U0001d794", "\U00000415", "\U00002d39", "\U000013ac", "\U0000a4f0", "\U000000c8", "\U000000c9", "\U000000ca", "\U000000cb", "\U00000112", "\U00000114", "\U00000116", "\U00000118", "\U0000011a", "\U0000018e", "\U00000190", "\U00000204", "\U00000206", "\U00000228", "\U00000246", "\U00000388", "\U0000042d", "\U000004ec", "\U00001e14", "\U00001e16", "\U00001e18", "\U00001e1a", "\U00001e1c", "\U00001eb8", "\U00001eba", "\U00001ebc", "\U00001ebe", "\U00001ec0", "\U00001ec2", "\U00001ec4", "\U00001ec6", "\U00001f18", "\U00001f19", "\U00001f1a", "\U00001f1b", "\U00001f1c", "\U00001f1d", "\U00001fc8", "\U00001fc9", "\U000000e8", "\U000000e9", "\U000000ea", "\U000000eb", "\U00000113", "\U00000115", "\U00000117", "\U00000119", "\U0000011b", "\U0000018f", "\U00000205", "\U00000207", "\U00000229", "\U00000247", "\U00000258", "\U0000025b", "\U0000025c", "\U0000025d", "\U0000025e", "\U00000364", "\U000003ad", "\U000003b5", "\U00000435", "\U0000044d", "\U000004ed", "\U00001e15", "\U00001e17", "\U00001e19", "\U00001e1b", "\U00001e1d", "\U00001eb9", "\U00001ebb", "\U00001ebd", "\U00001ebf", "\U00001ec1", "\U00001ec3", "\U00001ec5", "\U00001ec7", "\U00001f10", "\U00001f11", "\U00001f12", "\U00001f13", "\U00001f14", "\U00001f15", "\U00001f72", "\U00001f73" };
	std::vector<std::string> rs = { "\U00000403", "\U0000042f", "\U00000072", "\U0001d42b", "\U0001d45f", "\U0001d493", "\U0001d4c7", "\U0001d4fb", "\U0001d52f", "\U0001d563", "\U0001d597", "\U0001d5cb", "\U0001d5ff", "\U0001d633", "\U0001d667", "\U0001d69b", "\U0000ab47", "\U0000ab48", "\U00001d26", "\U00002c85", "\U00000433", "\U0000ab81", "\U0000211b", "\U0000211c", "\U0000211d", "\U0001d411", "\U0001d445", "\U0001d479", "\U0001d4e1", "\U0001d57d", "\U0001d5b1", "\U0001d5e5", "\U0001d619", "\U0001d64d", "\U0001d681", "\U000001a6", "\U000013a1", "\U000013d2", "\U000104b4", "\U00001587", "\U0000a4e3", "\U00000154", "\U00000156", "\U00000158", "\U00000210", "\U00000212", "\U0000024c", "\U00000280", "\U00000281", "\U00001e58", "\U00001e5a", "\U00001e5c", "\U00001e5e", "\U00002c64", "\U0000ff32", "\U000013a1", "\U00000155", "\U00000157", "\U00000159", "\U00000211", "\U00000213", "\U0000024d", "\U00000279", "\U0000027a", "\U0000027b", "\U0000027c", "\U0000027d", "\U000016b1", "\U00001875", "\U00001d72", "\U00001d73", "\U00001d89", "\U00001e59", "\U00001e5b", "\U00001e5d", "\U00001e5f", "\U0000ff52" };

	std::vector<std::vector<std::string>> s = { ns, is, gs, es, rs };

	for (auto x = 0;x < 5;x++) {
		for (auto chr : s[x]) {
			switch (x) {
				case 0: map.push_back(std::make_tuple('N', chr)); break;
				case 1: map.push_back(std::make_tuple('I', chr)); break;
				case 2: map.push_back(std::make_tuple('G', chr)); break;
				case 3: map.push_back(std::make_tuple('E', chr)); break;
				case 4: map.push_back(std::make_tuple('R', chr)); break;
			}
		}
	}

	auto matcher = ConfusableMatcher(map, true);

	std::unordered_set<char> ignore = { '_', '%', '$' };

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	int res = 0;
	for (auto x = 0;x < 5000000;x++) {
		auto ret = matcher.StringContains(
			"AAAAAAAAASSAFSAFNFNFNISFNSIFSIFJSDFUDSHF ASUF/|/__/|/___%/|/%I%%/|//|/%%%%%NNNN/|/NN__/|/N__𝘪G___%____$__G__𝓰𝘦Ѓ",
			"NIGGER",
			ignore,
			true,
			0);
		int r1, r2;
		std::tie(r1, r2) = ret;
		res += r1;
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << "result: " << res << " time " << duration << "ms, " << ((double)duration / 5000000) << "us per item" << std::endl;

	/*map.push_back(std::make_tuple('A', "BO"));
	map.push_back(std::make_tuple('A', "BI"));
	map.push_back(std::make_tuple('A', "BR"));
	map.push_back(std::make_tuple('A', "TR"));
	map.push_back(std::make_tuple('A', "TO"));
	map.push_back(std::make_tuple('B', "OOOOOOOOOO"));

	auto matcher = ConfusableMatcher(map, true);

	matcher.RemoveMapping('A', "BO");
	matcher.RemoveMapping('A', "BI");
	matcher.RemoveMapping('A', "BR");
	matcher.RemoveMapping('A', "TR");
	matcher.RemoveMapping('A', "TO");
	matcher.RemoveMapping('B', "OOOOOOOOOO");

	matcher.AddMapping('O', "PA", true);
	auto ret = matcher.StringContains("APAM", "AOM", { }, true, 0);
	int r1, r2;
	std::tie(r1, r2) = ret;
	std::cout << r1 << " " << r2 << std::endl;*/
}