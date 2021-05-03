#include "UnitTest.h"
#include <thread>

void Test1()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "T"));
	map.push_back(std::pair("I", "E"));
	map.push_back(std::pair("C", "S"));
	map.push_back(std::pair("E", "T"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("TEST", "NICE", opts);
	AssertMatch(res, 0, 4);
}

void Test2()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("V", "VA"));
	map.push_back(std::pair("V", "VO"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("VV", "VAVOVAVO", opts);
	AssertNoMatch(res);
	res = matcher.IndexOf("VAVOVAVO", "VV", opts);
	AssertMatchMulti(res, { 0, 4 }, { 3, 4 });
	opts.StartIndex = 4;
	res = matcher.IndexOf("VAVOVAVO", "VV", opts);
	AssertMatchMulti(res, { 4, 4 }, { 3, 4 });
	opts.StartIndex = 2;
	res = matcher.IndexOf("VAVOVAVO", "VV", opts);
	AssertMatchMulti(res, { 2, 4 }, { 3, 4 });
	opts.StartIndex = 3;
	res = matcher.IndexOf("VAVOVAVO", "VV", opts);
	AssertMatchMulti(res, { 4, 4 }, { 3, 4 });
}

void Test3()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("A", "\U00000002\U00000003"));
	map.push_back(std::pair("B", "\U000000FA\U000000FF"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("\U00000002\U00000003\U000000FA\U000000FF", "AB", opts);
	AssertMatch(res, 0, 6);
}

void Test4()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("S", "$"));
	map.push_back(std::pair("D", "[)"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, { "_", " " });
	auto res = matcher.IndexOf("A__ _ $$$[)D", "ASD", opts);
	AssertMatch(res, 0, 11);
}

void Test5()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("N", "/\\"));
	map.push_back(std::pair("I", "/"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("/\\/CE", "NICE", opts);
	AssertMatch(res, 0, 5);
}

void Test6()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("V", "\\/"));
	map.push_back(std::pair("I", "/"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("I/\\/AM", "INAN", opts);
	AssertNoMatch(res);
	res = matcher.IndexOf("I/\\/AM", "INAM", opts);
	AssertMatch(res, 0, 6);
	res = matcher.IndexOf("I/\\/AM", "IIVAM", opts);
	AssertMatch(res, 0, 6);
}

std::vector<std::pair<std::string, std::string>> GetDefaultMap()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "/[()[]]/"));
	map.push_back(std::pair("N", "\U000000f1"));
	map.push_back(std::pair("N", "|\\|"));
	map.push_back(std::pair("N", "\U00000245\U0000002f"));
	map.push_back(std::pair("N", "/IJ"));
	map.push_back(std::pair("N", "/|/"));

	std::vector<std::string> ns = { "\U000004c5", "\U000003a0", "\U00000418", "\U0001d427", "\U0001d45b", "\U0001d48f", "\U0001d4c3", "\U0001d4f7", "\U0001d52b", "\U0001d55f", "\U0001d593", "\U0001d5c7", "\U0001d5fb", "\U0001d62f", "\U0001d663", "\U0001d697", "\U00000578", "\U0000057c", "\U0000ff2e", "\U00002115", "\U0001d40d", "\U0001d441", "\U0001d475", "\U0001d4a9", "\U0001d4dd", "\U0001d511", "\U0001d579", "\U0001d5ad", "\U0001d5e1", "\U0001d615", "\U0001d649", "\U0001d67d", "\U0000039d", "\U0001d6b4", "\U0001d6ee", "\U0001d728", "\U0001d762", "\U0001d79c", "\U0000a4e0", "\U00000143", "\U00000145", "\U00000147", "\U0000014b", "\U0000019d", "\U000001f8", "\U00000220", "\U0000039d", "\U00001e44", "\U00001e46", "\U00001e48", "\U00001e4a", "\U000020a6", "\U00001f20", "\U00001f21", "\U00001f22", "\U00001f23", "\U00001f24", "\U00001f25", "\U00001f26", "\U00001f27", "\U00001f74", "\U00001f75", "\U00001f90", "\U00001f91", "\U00001f92", "\U00001f93", "\U00001f94", "\U00001f95", "\U00001f96", "\U00001f97", "\U00001fc2", "\U00001fc3", "\U00001fc4", "\U00001fc6", "\U00001fc7", "\U000000f1", "\U00000144", "\U00000146", "\U00000148", "\U00000149", "\U0000014a", "\U0000019e", "\U000001f9", "\U00000235", "\U00000272", "\U00000273", "\U00000274", "\U00001d70", "\U00001d87", "\U00001e45", "\U00001e47", "\U00001e49", "\U00001e4b" };
	std::vector<std::string> is = { "\U00001ec8", "\U00000079", "\U00000069", "\U00000031", "\U0000007c", "\U0000006c", "\U0000006a", "\U00000021", "\U0000002f", "\U0000005c\U0000005c", "\U0000ff49", "\U000000a1", "\U00002170", "\U00002139", "\U00002148", "\U0001d422", "\U0001d456", "\U0001d48a", "\U0001d4be", "\U0001d4f2", "\U0001d526", "\U0001d55a", "\U0001d58e", "\U0001d5c2", "\U0001d5f6", "\U0001d62a", "\U0001d65e", "\U0001d692", "\U00000131", "\U0001d6a4", "\U0000026a", "\U00000269", "\U000003b9", "\U00001fbe", "\U0000037a", "\U0001d6ca", "\U0001d704", "\U0001d73e", "\U0001d778", "\U0001d7b2", "\U00000456", "\U000024be", "\U0000a647", "\U000004cf", "\U0000ab75", "\U000013a5", "\U00000263", "\U00001d8c", "\U0000ff59", "\U0001d432", "\U0001d466", "\U0001d49a", "\U0001d4ce", "\U0001d502", "\U0001d536", "\U0001d56a", "\U0001d59e", "\U0001d5d2", "\U0001d606", "\U0001d63a", "\U0001d66e", "\U0001d6a2", "\U0000028f", "\U00001eff", "\U0000ab5a", "\U000003b3", "\U0000213d", "\U0001d6c4", "\U0001d6fe", "\U0001d738", "\U0001d772", "\U0001d7ac", "\U00000443", "\U000004af", "\U000010e7", "\U0000ff39", "\U0001d418", "\U0001d44c", "\U0001d480", "\U0001d4b4", "\U0001d4e8", "\U0001d51c", "\U0001d550", "\U0001d584", "\U0001d5b8", "\U0001d5ec", "\U0001d620", "\U0001d654", "\U0001d688", "\U000003a5", "\U000003d2", "\U0001d6bc", "\U0001d6f6", "\U0001d730", "\U0001d76a", "\U0001d7a4", "\U00002ca8", "\U00000423", "\U000004ae", "\U000013a9", "\U000013bd", "\U0000a4ec", "\U00000176", "\U00000178", "\U000001b3", "\U00000232", "\U0000024e", "\U0000028f", "\U00001e8e", "\U00001ef2", "\U00001ef4", "\U00001ef6", "\U00001ef8", "\U0000ff39", "\U000000cc", "\U000000cd", "\U000000ce", "\U000000cf", "\U00000128", "\U0000012a", "\U0000012c", "\U0000012e", "\U00000130", "\U00000196", "\U00000197", "\U000001cf", "\U00000208", "\U0000020a", "\U0000026a", "\U0000038a", "\U00000390", "\U00000399", "\U000003aa", "\U00000406", "\U0000040d", "\U00000418", "\U00000419", "\U000004e2", "\U000004e4", "\U00001e2c", "\U00001e2e", "\U00001ec8", "\U00001eca", "\U00001fd8", "\U00001fd9", "\U00002160", "\U0000ff29", "\U000030a7", "\U000030a8", "\U0000ff6a", "\U0000ff74", "\U000000ec", "\U000000ed", "\U000000ee", "\U000000ef", "\U00000129", "\U0000012b", "\U0000012d", "\U0000012f", "\U00000131", "\U000001d0", "\U00000209", "\U0000020b", "\U00000268", "\U00000269", "\U00000365", "\U000003af", "\U000003ca", "\U00000438", "\U00000439", "\U00000456", "\U0000045d", "\U000004e3", "\U000004e5", "\U00001e2d", "\U00001e2f", "\U00001ec9", "\U00001ecb", "\U00001f30", "\U00001f31", "\U00001f32", "\U00001f33", "\U00001f34", "\U00001f35", "\U00001f36", "\U00001f37", "\U00001f76", "\U00001f77", "\U00001fbe", "\U00001fd0", "\U00001fd1", "\U00001fd2", "\U00001fd3", "\U00001fd6", "\U00001fd7", "\U0000ff49", "\U00001d85", "\U00001e37", "\U00001e39", "\U00001e3b", "\U00001e3d", "\U000000fd", "\U000000ff", "\U00000177", "\U000001b4", "\U00000233", "\U0000024f", "\U0000028e", "\U000002b8", "\U00001e8f", "\U00001e99", "\U00001ef3", "\U00001ef5", "\U00001ef7", "\U00001ef9", "\U0000ff59" };
	std::vector<std::string> gs = { "\U0000006b", "\U00000067", "\U00000071", "\U00000034", "\U00000036", "\U00000039", "\U0000011f", "\U00000d6b", "\U0000ff47", "\U0000210a", "\U0001d420", "\U0001d454", "\U0001d488", "\U0001d4f0", "\U0001d524", "\U0001d558", "\U0001d58c", "\U0001d5c0", "\U0001d5f4", "\U0001d628", "\U0001d65c", "\U0001d690", "\U00000261", "\U00001d83", "\U0000018d", "\U00000581", "\U0001d406", "\U0001d43a", "\U0001d46e", "\U0001d4a2", "\U0001d4d6", "\U0001d50a", "\U0001d53e", "\U0001d572", "\U0001d5a6", "\U0001d5da", "\U00004e48", "\U0001d60e", "\U0001d642", "\U0001d676", "\U0000050c", "\U000013c0", "\U000013f3", "\U0000a4d6", "\U0000011c", "\U0000011e", "\U00000120", "\U00000122", "\U00000193", "\U000001e4", "\U000001e6", "\U000001f4", "\U0000029b", "\U00000393", "\U00000413", "\U00001e20", "\U0000ff27", "\U000013b6", "\U0000011d", "\U0000011f", "\U00000121", "\U00000123", "\U000001e5", "\U000001e7", "\U000001f5", "\U00000260", "\U00000261", "\U00000262", "\U00000040" };
	std::vector<std::string> es = { "\U00001ec0", "\U000003a3", "\U0000039e", "\U00000065", "\U00000033", "\U00000075", "\U0000212e", "\U0000ff45", "\U0000212f", "\U00002147", "\U0001d41e", "\U0001d452", "\U0001d486", "\U0001d4ee", "\U0001d522", "\U0001d556", "\U0001d58a", "\U0001d5be", "\U0001d5f2", "\U0001d626", "\U0001d65a", "\U0001d68e", "\U0000ab32", "\U00000435", "\U000004bd", "\U000022ff", "\U0000ff25", "\U00002130", "\U0001d404", "\U0001d438", "\U0001d46c", "\U0001d4d4", "\U0001d508", "\U0001d53c", "\U0001d570", "\U0001d5a4", "\U0001d5d8", "\U0001d60c", "\U0001d640", "\U0001d674", "\U00000395", "\U0001d6ac", "\U0001d6e6", "\U0001d720", "\U0001d75a", "\U0001d794", "\U00000415", "\U00002d39", "\U000013ac", "\U0000a4f0", "\U000000c8", "\U000000c9", "\U000000ca", "\U000000cb", "\U00000112", "\U00000114", "\U00000116", "\U00000118", "\U0000011a", "\U0000018e", "\U00000190", "\U00000204", "\U00000206", "\U00000228", "\U00000246", "\U00000388", "\U0000042d", "\U000004ec", "\U00001e14", "\U00001e16", "\U00001e18", "\U00001e1a", "\U00001e1c", "\U00001eb8", "\U00001eba", "\U00001ebc", "\U00001ebe", "\U00001ec0", "\U00001ec2", "\U00001ec4", "\U00001ec6", "\U00001f18", "\U00001f19", "\U00001f1a", "\U00001f1b", "\U00001f1c", "\U00001f1d", "\U00001fc8", "\U00001fc9", "\U000000e8", "\U000000e9", "\U000000ea", "\U000000eb", "\U00000113", "\U00000115", "\U00000117", "\U00000119", "\U0000011b", "\U0000018f", "\U00000205", "\U00000207", "\U00000229", "\U00000247", "\U00000258", "\U0000025b", "\U0000025c", "\U0000025d", "\U0000025e", "\U00000364", "\U000003ad", "\U000003b5", "\U00000435", "\U0000044d", "\U000004ed", "\U00001e15", "\U00001e17", "\U00001e19", "\U00001e1b", "\U00001e1d", "\U00001eb9", "\U00001ebb", "\U00001ebd", "\U00001ebf", "\U00001ec1", "\U00001ec3", "\U00001ec5", "\U00001ec7", "\U00001f10", "\U00001f11", "\U00001f12", "\U00001f13", "\U00001f14", "\U00001f15", "\U00001f72", "\U00001f73" };
	std::vector<std::string> rs = { "\U00000403", "\U0000042f", "\U00000072", "\U0001d42b", "\U0001d45f", "\U0001d493", "\U0001d4c7", "\U0001d4fb", "\U0001d52f", "\U0001d563", "\U0001d597", "\U0001d5cb", "\U0001d5ff", "\U0001d633", "\U0001d667", "\U0001d69b", "\U0000ab47", "\U0000ab48", "\U00001d26", "\U00002c85", "\U00000433", "\U0000ab81", "\U0000211b", "\U0000211c", "\U0000211d", "\U0001d411", "\U0001d445", "\U0001d479", "\U0001d4e1", "\U0001d57d", "\U0001d5b1", "\U0001d5e5", "\U0001d619", "\U0001d64d", "\U0001d681", "\U000001a6", "\U000013a1", "\U000013d2", "\U000104b4", "\U00001587", "\U0000a4e3", "\U00000154", "\U00000156", "\U00000158", "\U00000210", "\U00000212", "\U0000024c", "\U00000280", "\U00000281", "\U00001e58", "\U00001e5a", "\U00001e5c", "\U00001e5e", "\U00002c64", "\U0000ff32", "\U000013a1", "\U00000155", "\U00000157", "\U00000159", "\U00000211", "\U00000213", "\U0000024d", "\U00000279", "\U0000027a", "\U0000027b", "\U0000027c", "\U0000027d", "\U000016b1", "\U00001875", "\U00001d72", "\U00001d73", "\U00001d89", "\U00001e59", "\U00001e5b", "\U00001e5d", "\U00001e5f", "\U0000ff52" };

	std::vector<std::vector<std::string>> s = { ns, is, gs, es, rs };

	for (auto x = 0;x < 5;x++) {
		for (auto chr : s[x]) {
			switch (x) {
				case 0: map.push_back(std::pair("N", chr)); break;
				case 1: map.push_back(std::pair("I", chr)); break;
				case 2: map.push_back(std::pair("G", chr)); break;
				case 3: map.push_back(std::pair("E", chr)); break;
				case 4: map.push_back(std::pair("R", chr)); break;
			}
		}
	}

	return map;
}

void Test7()
{
	auto map = GetDefaultMap();

	std::string in = "AAAAAAAAASSAFSAFNFNFNISFNSIFSIFJSDFUDSHF ASUF/|/__/|/___%/|/%I%%/|//|/%%%%%NNNN/|/NN__/|/N__𝘪G___%____$__G__𝓰𝘦Ѓ";

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, { "_", "%", "$" });
	auto res = matcher.IndexOf(in, "NIGGER", opts);

	AssertMatchMulti(res, { 64, 89 }, { 57, 32 });
}

ConfusableMatcher *LidlNormalizerCMSetup()
{
	auto map = GetDefaultMap();

	// Additional test data
	std::vector<std::string> keys = { 
		"A", "A", "A", "A", "B", "U", "U", "O", "O", "A", "A",
		"A", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
		"Z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"U", "A", " ", "S", "M", "O", "L", "N", "A", "T", "I", "O", "N", "N", "I", "G", "N", "I", "FREE", "AE"
	};
	std::vector<std::string> vals = {
		"\U00000105", "\U0000ab31", "\U00001d43", "\U000000e5", "\U0000249d", "\U000000fc", "\U000000dc", "\U000000f6", "\U000000d6", "\U000000e4", "\U000000c4", "\U0000249c", "\U0000249e", "\U0000249f", "\U000024a0", "\U000024a1", "\U000024a2", "\U000024a3", "\U000024a4", "\U000024a5", "\U000024a6", "\U000024a7", "\U000024a8", "\U000024a9", "\U000024aa", "\U000024ab", "\U000024ac", "\U000024ad", "\U000024ae", "\U000024af", "\U000024b0", "\U000024b1", "\U000024b2", "\U000024b3", "\U000024b4", "\U000024cf", "\U000024d0", "\U000024d1", "\U000024d2", "\U000024d3", "\U000024d4", "\U000024d5", "\U000024d6", "\U000024d7", "\U000024d8", "\U000024d9", "\U000024da", "\U000024db", "\U000024dc", "\U000024dd", "\U000024de", "\U000024df", "\U000024e0", "\U000024e1", "\U000024e2", "\U000024e3", "\U000024e4", "\U000024e5", "\U000024e6", "\U000024e7", "\U000024e8", "\U000024e9", "\U000024ea", "\U0001d552", "\U0001d553", "\U0001d554", "\U0001d555", "\U0001d556", "\U0001d557", "\U0001d558", "\U0001d559", "\U0001d55a", "\U0001d55b", "\U0001d55c", "\U0001d55d", "\U0001d55e", "\U0001d55f", "\U0001d560", "\U0001d561", "\U0001d562", "\U0001d563", "\U0001d564", "\U0001d565", "\U0001d566", "\U0001d567", "\U0001d568", "\U0001d569", "\U0001d56a", "\U0001d56b", "\U0001f130", "\U0001f131", "\U0001f132", "\U0001f133", "\U0001f134", "\U0001f135", "\U0001f136", "\U0001f137", "\U0001f138", "\U0001f139", "\U0001f13a", "\U0001f13b", "\U0001f13c", "\U0001f13d", "\U0001f13e", "\U0001f13f", "\U0001f140", "\U0001f141", "\U0001f142", "\U0001f143", "\U0001f144", "\U0001f145", "\U0001f146", "\U0001f147", "\U0001f148", "\U0001f149", "\U000020b3", "\U00000e3f", "\U000020b5", "\U00000110", "\U00000246", "\U000020a3", "\U000020b2", "\U00002c67", "\U00000142", "\U0000004a", "\U000020ad", "\U00002c60", "\U000020a5", "\U000020a6", "\U000000d8", "\U000020b1", "\U00000051", "\U00002c64", "\U000020b4", "\U000020ae", "\U00000244", "\U00000056", "\U000020a9", "\U000004fe", "\U0000024e", "\U00002c6b", "\U0001d586", "\U0001d587", "\U0001d588", "\U0001d589", "\U0001d58a", "\U0001d58b", "\U0001d58c", "\U0001d58d", "\U0001d58e", "\U0001d58f", "\U0001d590", "\U0001d591", "\U0001d592", "\U0001d593", "\U0001d594", "\U0001d595", "\U0001d596", "\U0001d597", "\U0001d598", "\U0001d599", "\U0001d59a", "\U0001d59b", "\U0001d59c", "\U0001d59d", "\U0001d59e", "\U0001d59f", "\U0001f170", "\U0001f171", "\U0001f172", "\U0001f173", "\U0001f174", "\U0001f175", "\U0001f176", "\U0001f177", "\U0001f178", "\U0001f179", "\U0001f17a", "\U0001f17b", "\U0001f17c", "\U0001f17d", "\U0001f17e", "\U0001f17f", "\U0001f180", "\U0001f181", "\U0001f182", "\U0001f183", "\U0001f184", "\U0001f185", "\U0001f186", "\U0001f187", "\U0001f188", "\U0001f189", "\U0001f1fa", "\U0001f1e6", " ", "\U000002e2", "\U00001d50", "\U00001d52", "\U000002e1", "\U0000207f", "\U00001d43", "\U00001d57", "\U00001da6", "\U00001d52", "\U0000207f", "\U0000041d", "\U00000438", "\U00000433", "\U0001F1F3", "\U0001F1EE", "\U0001f193", "\U00001d2d"
	};
	
	for (auto x = 0;x < keys.size();x++)
		map.push_back(std::pair(keys[x], vals[x]));

	auto matcher = new ConfusableMatcher(map, { });

	return matcher;
}

std::vector<std::tuple<std::pair<std::string, std::string>, std::pair<int, int>>> LidlNormalizerData()
{
	return {
		std::make_tuple(std::pair<std::string, std::string>("\U00000105", "A"), std::pair<int, int>(0, 2)),
		std::make_tuple(std::pair<std::string, std::string>("\U0000ab31", "A"), std::pair<int, int>(0, 3)),
		std::make_tuple(std::pair<std::string, std::string>("\U00001d43", "A"), std::pair<int, int>(0, 3)),
		std::make_tuple(std::pair<std::string, std::string>("abc \U000000e5 def", "ABC A DEF"), std::pair<int, int>(0, 10)),
		std::make_tuple(std::pair<std::string, std::string>("\U000002e2\U00001d50\U00001d52\U000002e1 \U0000207f\U00001d43\U00001d57\U00001da6\U00001d52\U0000207f", "SMOL NATION"), std::pair<int, int>(0, 29)),
		std::make_tuple(std::pair<std::string, std::string>("\U0000041d\U00000438\U00000433", "NIG"), std::pair<int, int>(0, 6)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001f1fa\U0001f1e6XD", "UAXD"), std::pair<int, int>(0, 10)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001f193 ICE", "FREE ICE"), std::pair<int, int>(0, 8)),
		std::make_tuple(std::pair<std::string, std::string>("chocolate \U0001F1F3\U0001F1EEb", "CHOCOLATE NIB"), std::pair<int, int>(0, 19)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001f171lueberry", "BLUEBERRY"), std::pair<int, int>(0, 12)),
		std::make_tuple(std::pair<std::string, std::string>("\U0000249d", "B"), std::pair<int, int>(0, 3)),
		std::make_tuple(std::pair<std::string, std::string>("\U000000fc \U000000dc \U000000f6 \U000000d6 \U000000e4 \U000000c4", "U U O O A A"), std::pair<int, int>(0, 17)),
		std::make_tuple(std::pair<std::string, std::string>("\U00001d2d", "AE"), std::pair<int, int>(0, 3)),
		std::make_tuple(std::pair<std::string, std::string>("\U0000249c \U0000249d \U0000249e \U0000249f \U000024a0 \U000024a1 \U000024a2 \U000024a3 \U000024a4 \U000024a5 \U000024a6 \U000024a7 \U000024a8 \U000024a9 \U000024aa \U000024ab \U000024ac \U000024ad \U000024ae \U000024af \U000024b0 \U000024b1 \U000024b2 \U000024b3 \U000024b4", "A B C D E F G H I J K L M N O P Q R S T U V W X Y"), std::pair<int, int>(0, 99)),
		std::make_tuple(std::pair<std::string, std::string>("\U000024cf\U000024d0\U000024d1\U000024d2\U000024d3\U000024d4\U000024d5\U000024d6\U000024d7\U000024d8\U000024d9\U000024da\U000024db\U000024dc\U000024dd\U000024de\U000024df\U000024e0\U000024e1\U000024e2\U000024e3\U000024e4\U000024e5\U000024e6\U000024e7\U000024e8\U000024e9\U000024ea", "ZABCDEFGHIJKLMNOPQRSTUVWXYZ0"), std::pair<int, int>(0, 84)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001d552\U0001d553\U0001d554\U0001d555\U0001d556\U0001d557\U0001d558\U0001d559\U0001d55a\U0001d55b\U0001d55c\U0001d55d\U0001d55e\U0001d55f\U0001d560\U0001d561\U0001d562\U0001d563\U0001d564\U0001d565\U0001d566\U0001d567\U0001d568\U0001d569\U0001d56a\U0001d56b", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"), std::pair<int, int>(0, 104)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001f130\U0001f131\U0001f132\U0001f133\U0001f134\U0001f135\U0001f136\U0001f137\U0001f138\U0001f139\U0001f13a\U0001f13b\U0001f13c\U0001f13d\U0001f13e\U0001f13f\U0001f140\U0001f141\U0001f142\U0001f143\U0001f144\U0001f145\U0001f146\U0001f147\U0001f148\U0001f149", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"), std::pair<int, int>(0, 104)),
		std::make_tuple(std::pair<std::string, std::string>("\U000020b3\U00000e3f\U000020b5\U00000110\U00000246\U000020a3\U000020b2\U00002c67\U00000142J\U000020ad\U00002c60\U000020a5\U000020a6\U000000d8\U000020b1Q\U00002c64\U000020b4\U000020ae\U00000244V\U000020a9\U000004fe\U0000024e\U00002c6b", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"), std::pair<int, int>(0, 65)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001d586\U0001d587\U0001d588\U0001d589\U0001d58a\U0001d58b\U0001d58c\U0001d58d\U0001d58e\U0001d58f\U0001d590\U0001d591\U0001d592\U0001d593\U0001d594\U0001d595\U0001d596\U0001d597\U0001d598\U0001d599\U0001d59a\U0001d59b\U0001d59c\U0001d59d\U0001d59e\U0001d59f", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"), std::pair<int, int>(0, 104)),
		std::make_tuple(std::pair<std::string, std::string>("\U0001f170\U0001f171\U0001f172\U0001f173\U0001f174\U0001f175\U0001f176\U0001f177\U0001f178\U0001f179\U0001f17a\U0001f17b\U0001f17c\U0001f17d\U0001f17e\U0001f17f\U0001f180\U0001f181\U0001f182\U0001f183\U0001f184\U0001f185\U0001f186\U0001f187\U0001f188\U0001f189", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"), std::pair<int, int>(0, 104))
	};
}

CMOptions LidlNormalizerOpts()
{
	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;

	return opts;
}

void LidlNormalizerTests()
{
	auto matcher = LidlNormalizerCMSetup();
	auto data = LidlNormalizerData();
	auto opts = LidlNormalizerOpts();

	for (auto entry : data) {
		std::pair<std::string, std::string> chr;
		std::pair<int, int> eq;
		std::tie(chr, eq) = entry;
		auto res = matcher->IndexOf(chr.first, chr.second, opts);
		AssertMatch(res, eq.first, eq.second);
	}

	delete matcher;
}

void Test8()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	auto matcher = ConfusableMatcher(map, { "\U00000332", "\U00000305", "[", "]" });
	auto res = matcher.IndexOf(
		"[̲̅a̲̅][̲̅b̲̅][̲̅c̲̅][̲̅d̲̅][̲̅e̲̅][̲̅f̲̅][̲̅g̲̅][̲̅h̲̅][̲̅i̲̅][̲̅j̲̅][̲̅k̲̅][̲̅l̲̅][̲̅m̲̅][̲̅n̲̅][̲̅o̲̅][̲̅p̲̅][̲̅q̲̅][̲̅r̲̅][̲̅s̲̅][̲̅t̲̅][̲̅u̲̅][̲̅v̲̅][̲̅w̲̅][̲̅x̲̅][̲̅y̲̅][̲̅z̲̅][̲̅0̲̅][̲̅1̲̅][̲̅2̲̅][̲̅3̲̅][̲̅4̲̅][̲̅5̲̅][̲̅6̲̅][̲̅7̲̅][̲̅8̲̅][̲̅9̲̅][̲̅0̲̅]",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890",
		opts);
	AssertMatch(res, 5, 397);
}

void Test9()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("B", "A"));
	map.push_back(std::pair("B", "AB"));
	map.push_back(std::pair("B", "ABC"));
	map.push_back(std::pair("B", "ABCD"));
	map.push_back(std::pair("B", "ABCDE"));
	map.push_back(std::pair("B", "ABCDEF"));
	map.push_back(std::pair("B", "ABCDEFG"));
	map.push_back(std::pair("B", "ABCDEFGH"));
	map.push_back(std::pair("B", "ABCDEFGHI"));
	map.push_back(std::pair("B", "ABCDEFGHIJ"));
	map.push_back(std::pair("B", "ABCDEFGHIJK"));
	map.push_back(std::pair("B", "ABCDEFGHIJKL"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLM"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMN"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNO"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOP"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOPQ"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOPQR"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOPQRS"));

	CMOptions opts = { };
	opts.StatePushLimit = 1000;
	auto matcher = ConfusableMatcher(map, { });

	auto res = matcher.IndexOf(
		"ABCDEFGHIJKLMNOPQRS",
		"B",
		opts);
	AssertMatchMulti(res, { 0, 1 }, { 1, 1 });

	map.clear();
	map.push_back(std::pair("B", "A"));
	map.push_back(std::pair("B", "AB"));
	map.push_back(std::pair("B", "ABC"));
	map.push_back(std::pair("B", "ABCD"));
	map.push_back(std::pair("B", "ABCDE"));
	map.push_back(std::pair("B", "ABCDEF"));
	map.push_back(std::pair("B", "ABCDEFG"));
	map.push_back(std::pair("B", "ABCDEFGH"));
	map.push_back(std::pair("B", "ABCDEFGHI"));
	map.push_back(std::pair("B", "ABCDEFGHIJ"));
	map.push_back(std::pair("B", "ABCDEFGHIJK"));
	map.push_back(std::pair("B", "ABCDEFGHIJKL"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLM"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMN"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNO"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOPQ"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOPQR"));
	map.push_back(std::pair("B", "ABCDEFGHIJKLMNOPQRS"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ0123456789"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ012345678"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ01234567"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ0123456"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ012345"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ01234"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ0123"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ012"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ01"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ0"));
	map.push_back(std::pair("B", "PQRSTUVWXYZ"));
	map.push_back(std::pair("B", "PQRSTUVWXY"));
	map.push_back(std::pair("B", "PQRSTUVWX"));
	map.push_back(std::pair("B", "PQRSTUVW"));
	map.push_back(std::pair("B", "PQRSTUV"));
	map.push_back(std::pair("B", "PQRSTU"));
	map.push_back(std::pair("B", "PQRST"));
	map.push_back(std::pair("B", "PQRS"));
	map.push_back(std::pair("B", "PQR"));
	map.push_back(std::pair("B", "PQ"));
	map.push_back(std::pair("B", "P"));

	auto matcher2 = ConfusableMatcher(map, { });
	
	res = matcher2.IndexOf(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",
		"BB",
		opts);
	AssertMatch(res, 0, 2);

	opts.MatchRepeating = true;
	opts.StatePushLimit = 2000;
	res = matcher2.IndexOf(
		"PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789PQRSTUVWXYZ0123456789",
		"BBBBBBBBBBBBBBBBBBBBBBBBBBB",
		opts);
	ASSERT_EQUAL(res.Start, 0);
	ASSERT(res.Size >= 547 && res.Size <= 567);
}

void Test10()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf(":)", "", opts);
	AssertMatch(res, 0, 0);

	res = matcher.IndexOf("", ":)", opts);
	AssertNoMatch(res);
}

void Test11()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("A", "A"));
	map.push_back(std::pair("A", "A"));
	map.push_back(std::pair("A", "A"));
	map.push_back(std::pair("A", "A"));

	auto matcher = ConfusableMatcher(map, { });

	CMOptions opts = { };
	opts.MatchRepeating = true;
	opts.StatePushLimit = 1000;
	auto res = matcher.IndexOf("ABAAA", "ABAR", opts);
	AssertNoMatch(res);
}

void Test12()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	auto matcher = ConfusableMatcher(map, { });

	auto res = matcher.IndexOf("A", "A", opts);
	AssertMatch(res, 0, 1);

	auto matcher2 = ConfusableMatcher(map, { }, false);
	res = matcher2.IndexOf("A", "A", opts);
	AssertNoMatch(res);
}

void Test13()
{
	auto test = [](std::string k, std::string v)
	{
		std::vector<std::pair<std::string, std::string>> map;
		map.push_back(std::pair(k, v));
		auto matcher = ConfusableMatcher(map, { });
	};

	ASSERT_THROWS(test("", "?"), std::exception);
	ASSERT_THROWS(test("?", ""), std::exception);
	ASSERT_THROWS(test("", ""), std::exception);
	ASSERT_THROWS(test(std::string("\x00", 1), "?"), std::exception);
	ASSERT_THROWS(test("?", std::string("\x00", 1)), std::exception);
	ASSERT_THROWS(test(std::string("\x01", 1), "?"), std::exception);
	ASSERT_THROWS(test("?", std::string("\x01", 1)), std::exception);
	ASSERT_THROWS(test(std::string("\x00", 1), std::string("\x01", 1)), std::exception);
	ASSERT_THROWS(test(std::string("\x00", 1), std::string("\x00", 1)), std::exception);
	ASSERT_THROWS(test(std::string("\x01", 1), std::string("\x00", 1)), std::exception);
	ASSERT_THROWS(test(std::string("\x01", 1), std::string("\x01", 1)), std::exception);
	ASSERT_THROWS(test(std::string("\x01\x00", 2), std::string("\x00\x01", 2)), std::exception);
	ASSERT_THROWS(test(std::string("A\x00", 2), std::string("\x00""A", 2)), std::exception);
	ASSERT_THROWS(test(std::string("\x01\x00", 2), std::string("\x00\x01", 2)), std::exception);
	test(std::string("A\x00", 2), std::string("A\x01", 2));
	test(std::string("A\x01", 2), std::string("A\x00", 2));
	test(std::string("A\x00", 2), std::string("A\x00", 2));
	test(std::string("A\x01", 2), std::string("A\x01", 2));
}

void Test14()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, { });
	auto running = true;

	std::thread t1([&matcher, &running, opts] {
		while (running) {
			matcher.IndexOf("ASD", "ZXC", opts);
		}
	});
	std::thread t2([&matcher, &running, opts] {
		while (running) {
			matcher.IndexOf("LIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH NLIGGER CMONBRUH", "NIGGAR", opts);
		}
	});
	std::thread t3([&matcher, &running, opts] {
		while (running) {
			matcher.IndexOf("NIGGAR", "NIGGER", opts);
		}
	});

	auto now = std::chrono::high_resolution_clock::now();

	while ((std::chrono::high_resolution_clock::now() - now) < std::chrono::seconds(10)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	running = false;

	t1.join();
	t2.join();
	t3.join();
}

void Test15()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair(".", "."));

	auto matcher = ConfusableMatcher(map, { "." });

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto res = matcher.IndexOf("FOLLOWONBOT.COM", "FOLLOWONBOT.COM", opts);
	AssertMatch(res, 0, 15);
}

void Test16()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("N", "\\/\\"));

	auto matcher = ConfusableMatcher(map, { });

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto res = matcher.IndexOf("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/", "NNNNNNNNNNNNNNNA", opts);
	ASSERT_EQUAL(STATE_PUSH_LIMIT_EXCEEDED, res.Status);
}

void Test17()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("N", "_"));
	map.push_back(std::pair("N", "__"));

	auto matcher = ConfusableMatcher(map, {});

	CMOptions opts = { };
	opts.MatchRepeating = true;
	opts.StatePushLimit = 100000;
	auto res = matcher.IndexOf("NNNNN__N_NN___NNNNNN_NN_N__NNNN__N_NNNNNICE", "NIRE", opts);
	AssertNoMatch(res);
}

void Test18()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("N", "12345"));
	map.push_back(std::pair("A", "1"));
	map.push_back(std::pair("A", "5"));
	map.push_back(std::pair("A", "234"));

	auto matcher = ConfusableMatcher(map, {});

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto res = matcher.IndexOf("N12345M", "NAM", opts);
	AssertMatch(res, 0, 7);
}

void Test19()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("A", "1"));
	map.push_back(std::pair("B", "1"));
	map.push_back(std::pair("C", "1"));

	auto matcher = ConfusableMatcher(map, {});

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto res = matcher.IndexOf("111111", "ABC", opts);
	AssertMatch(res, 0, 3);
}

void Test20()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("A", "1"));
	map.push_back(std::pair("B", "1"));
	map.push_back(std::pair("C", "1"));

	auto matcher = ConfusableMatcher(map, {}, false);

	StackVector<CMString> output;
	matcher.GetKeyMappings("A", output);
	ASSERT_EQUAL("1", output.GetElement(0).View());

	output.Reset();
	matcher.GetKeyMappings("B", output);
	ASSERT_EQUAL("1", output.GetElement(0).View());

	output.Reset();
	matcher.GetKeyMappings("C", output);
	ASSERT_EQUAL("1", output.GetElement(0).View());
}

void Test21()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("1", "AB"));
	map.push_back(std::pair("1", "CD"));
	map.push_back(std::pair("2", "EEE"));

	auto matcher = ConfusableMatcher(map, {}, false);

	StackVector<CMString> output;
	matcher.GetKeyMappings("1", output);
	output.Reset();
	matcher.GetKeyMappings("1", output);

	ASSERT(output.GetElement(0).View() == "AB" || output.GetElement(0).View() == "CD");
	ASSERT(output.GetElement(1).View() == "AB" || output.GetElement(1).View() == "CD");

	output.Reset();
	matcher.GetKeyMappings("2", output);

	ASSERT_EQUAL("EEE", output.GetElement(0).View());
}

void Test22()
{
	std::vector<std::pair<std::string, std::string>> map;
	for (auto x = 0;x < 500;x++)
		map.push_back(std::pair("123", std::to_string(x)));

	auto matcher = ConfusableMatcher(map, {}, false);

	StackVector<CMString> output;
	matcher.GetKeyMappings("123", output);

	std::bitset<500> accountedFor;

	for (auto x = 0;x < 500;x++) {
		accountedFor[std::atoi(output.GetElement(x).Str)] = true;
	}

	for (auto x = 0;x < 500;x++) {
		ASSERT(accountedFor[x]);
	}

	ASSERT_EQUAL(500, output.Size());
}

void Test23()
{
	auto map = GetDefaultMap();

	std::string in = "AA BB CC AA FF AA RR";

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf(in, "AA", opts);
	AssertMatch(res, 0, 2);

	opts.StartIndex = in.size() - 1;
	opts.StartFromEnd = true;
	res = matcher.IndexOf(in, "AA", opts);
	AssertMatch(res, 15, 2);
}

void Test24()
{
	auto map = GetDefaultMap();

	std::string in = "DASD";

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	opts.StartIndex = in.size() - 1;
	opts.StartFromEnd = true;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf(in, "D", opts);
	AssertMatch(res, 3, 1);
}

void Test25()
{
	auto map = GetDefaultMap();

	std::string in = "ASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASASB";

	CMOptions opts = { };
	opts.MatchRepeating = true;
	opts.StartFromEnd = false;
	opts.StatePushLimit = 20;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf(in, "ASB", opts);
	ASSERT_EQUAL(STATE_PUSH_LIMIT_EXCEEDED, res.Status);

	opts.StartFromEnd = true;
	opts.StartIndex = in.size() - 1;
	res = matcher.IndexOf(in, "ASB", opts);
	AssertMatch(res, 92, 3);
}

void Test26()
{
	auto map = GetDefaultMap();

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("AAA", "A", opts);
	AssertMatch(res, 0, 3);
}

void Test27()
{
	auto map = GetDefaultMap();

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("BB AAA", "A", opts);
	AssertMatch(res, 3, 3);
}

void Test28()
{
	auto map = GetDefaultMap();

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("N|\\|NC", "N", opts);
	AssertMatch(res, 0, 5);
}

void Test29()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("N", "//A"));
	map.push_back(std::pair("N", "//"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchRepeating = true;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("N/\\///AN", "N", opts);
	AssertMatch(res, 0, 8);
}

void Test30()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("N", "/"));

	CMOptions opts = { };
	opts.MatchRepeating = true;
	opts.StatePushLimit = 60;
	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("N////////////////////////////////////////////////", "N", opts);

	ASSERT_EQUAL(0, res.Start);
	ASSERT_GREATER(res.Size, 10);
}

void Test31()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;

	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("X", "X", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("aX", "X", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);
	ASSERT_EQUAL(1, res.Start);
	ASSERT_EQUAL(1, res.Size);

	res = matcher.IndexOf("Xa", "X", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
	ASSERT_EQUAL(0, res.Start);
	ASSERT_EQUAL(1, res.Size);

	res = matcher.IndexOf("a X", "X", opts);
	AssertMatch(res, 2, 1);

	res = matcher.IndexOf("X a", "X", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("X;duper", "X", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("yes\uFEFFX", "X", opts);
	AssertMatch(res, 6, 1);
}

void Test32()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("QQQ", "Q", opts);
	AssertMatch(res, 0, 3);

	res = matcher.IndexOf("aQQQ", "Q", opts);
	ASSERT(res.Status == WORD_BOUNDARY_FAIL_START || res.Status == WORD_BOUNDARY_FAIL_END);
	AssertMatchMulti(res, { 1, 1, 1, 2, 2, 3 }, { 1, 2, 3, 1, 2, 1 }, res.Status);

	res = matcher.IndexOf("QQQa", "Q", opts);

	ASSERT(res.Status == WORD_BOUNDARY_FAIL_START || res.Status == WORD_BOUNDARY_FAIL_END);
	AssertMatchMulti(res, { 0, 0, 0, 1, 1, 2 }, { 1, 2, 3, 1, 2, 1 }, res.Status);

	res = matcher.IndexOf("a QQQ", "Q", opts);
	AssertMatch(res, 2, 3);

	res = matcher.IndexOf("QQQ a", "Q", opts);
	AssertMatch(res, 0, 3);

	res = matcher.IndexOf("QQQ;duper", "Q", opts);
	AssertMatch(res, 0, 3);

	res = matcher.IndexOf("yes\u202FQQQ", "Q", opts);
	AssertMatch(res, 6, 3);
}

void Test33()
{
	std::vector<std::pair<std::string, std::string>> map;

	std::string in = "a QBQQ";

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;
	opts.StartFromEnd = true;
	opts.StartIndex = in.size() - 1;

	auto matcher = ConfusableMatcher(map, { "B" });
	auto res = matcher.IndexOf(in, "Q", opts);
	AssertMatch(res, 2, 4);
}

void Test34()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;

	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("SUPER", "SUPER", opts);
	AssertMatch(res, 0, 5);

	res = matcher.IndexOf("aSUPER", "SUPER", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);
	ASSERT_EQUAL(1, res.Start);
	ASSERT_EQUAL(5, res.Size);

	res = matcher.IndexOf("SUPERa", "SUPER", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
	ASSERT_EQUAL(0, res.Start);
	ASSERT_EQUAL(5, res.Size);

	res = matcher.IndexOf("a SUPER", "SUPER", opts);
	AssertMatch(res, 2, 5);

	res = matcher.IndexOf("SUPER a", "SUPER", opts);
	AssertMatch(res, 0, 5);

	res = matcher.IndexOf("SUPER;duper", "SUPER", opts);
	AssertMatch(res, 0, 5);

	res = matcher.IndexOf("yes\u202FSUPER", "SUPER", opts);
	AssertMatch(res, 6, 5);
}

void Test35()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair(" ", " "));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;

	auto matcher = ConfusableMatcher(map, { " " });
	auto res = matcher.IndexOf("a Q Q f", "Q Q", opts);
	AssertMatch(res, 2, 3);

	res = matcher.IndexOf("aQ Q f", "Q Q", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);
	ASSERT_EQUAL(1, res.Start);
	ASSERT_EQUAL(3, res.Size);

	res = matcher.IndexOf("a Q Qf", "Q Q", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
	ASSERT_EQUAL(2, res.Start);
	ASSERT_EQUAL(3, res.Size);
}

void Test36()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair(" ", " "));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;

	auto matcher = ConfusableMatcher(map, { " " });
	auto res = matcher.IndexOf("a Q Q Q f", "Q Q", opts);
	AssertMatchMulti(res, { 2, 4 }, { 3, 3 });
}

void Test37()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("a", "a"));
	map.push_back(std::pair("s", "s"));
	map.push_back(std::pair(" ", " "));
	map.push_back(std::pair("i", "i"));
	map.push_back(std::pair("m", "m"));
	map.push_back(std::pair("p", "p"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { " " });
	auto res = matcher.IndexOf("as simp", "simp", opts);
	AssertMatch(res, 3, 4);
}

void Test38()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;

	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("a", "", opts);
	AssertMatch(res, 0, 0);

	res = matcher.IndexOf("", "", opts);
	AssertMatch(res, 0, 0);

	res = matcher.IndexOf("", "a", opts);
	AssertNoMatch(res);
}

void Test39()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { "f" });
	auto res = matcher.IndexOf("AABC", "ABC", opts);
	AssertMatch(res, 0, 4);

	res = matcher.IndexOf("AfABC", "ABC", opts);
	AssertMatch(res, 0, 5);

	res = matcher.IndexOf("ABCC", "ABC", opts);
	AssertMatch(res, 0, 4);

	res = matcher.IndexOf("ABCfC", "ABC", opts);
	AssertMatch(res, 0, 5);
}

void Test40()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;
	opts.StartFromEnd = true;

	auto matcher = ConfusableMatcher(map, { "f" });
	opts.StartIndex = 3;
	auto res = matcher.IndexOf("AABC", "ABC", opts);
	AssertMatch(res, 0, 4);

	opts.StartIndex = 4;
	res = matcher.IndexOf("AfABC", "ABC", opts);
	AssertMatch(res, 0, 5);

	opts.StartIndex = 3;
	res = matcher.IndexOf("ABCC", "ABC", opts);
	AssertMatch(res, 0, 4);

	opts.StartIndex = 4;
	res = matcher.IndexOf("ABCfC", "ABC", opts);
	AssertMatch(res, 0, 5);
}

void Test41()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("i", "1"));
	map.push_back(std::pair("s", "s"));
	map.push_back(std::pair("m", "m"));
	map.push_back(std::pair("p", "p"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { " " });
	auto res = matcher.IndexOf("agdhsjs s 1 mmm ppps dhsjdhsd", "simps", opts);
	AssertMatch(res, 8, 12);
}

void Test42()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { "░" });
	auto res = matcher.IndexOf("░S░I░M░P░", "SIMP", opts);
	AssertMatch(res, 3, 13);

	auto matcher2 = ConfusableMatcher(map, { "Ž" });
	res = matcher2.IndexOf("ŽSŽIŽMŽPŽ", "SIMP", opts);
	AssertMatch(res, 2, 12);
}

void Test43()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("S", "░"));
	map.push_back(std::pair("S", "Ž"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { "░" });
	auto res = matcher.IndexOf("░S░I░M░P░", "SIMP", opts);
	AssertMatch(res, 0, 16);

	auto matcher2 = ConfusableMatcher(map, { "Ž" });
	res = matcher2.IndexOf("ŽSŽIŽMŽPŽ", "SIMP", opts);
	AssertMatch(res, 0, 14);
}

void Test44()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair(" ", " "));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { });
	auto res = matcher.IndexOf("A  B", " B", opts);
	AssertMatch(res, 2, 2);
}

void Test45()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { "X" });
	auto res = matcher.IndexOf("XXABC", "ABC", opts);
	AssertMatch(res, 2, 3);

	res = matcher.IndexOf("X XABC", "ABC", opts);
	AssertMatch(res, 3, 3);

	res = matcher.IndexOf("X૰XABC", "ABC", opts);
	AssertMatch(res, 5, 3);

	res = matcher.IndexOf(" X XABC", "ABC", opts);
	AssertMatch(res, 4, 3);

	res = matcher.IndexOf(" XXABC", "ABC", opts);
	AssertMatch(res, 3, 3);

	res = matcher.IndexOf(" XX ABC", "ABC", opts);
	AssertMatch(res, 4, 3);

	res = matcher.IndexOf("XXXX XXABC", "ABC", opts);
	AssertMatch(res, 7, 3);
}

void Test46()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { });

	auto res = matcher.IndexOf("!A", "A", opts);
	AssertMatch(res, 1, 1);
	
	res = matcher.IndexOf("FSD!XCV", "XCV", opts);
	AssertMatch(res, 4, 3);

	res = matcher.IndexOf("¶A", "A", opts);
	AssertMatch(res, 2, 1);

	res = matcher.IndexOf("૰A", "A", opts);
	AssertMatch(res, 3, 1);

	res = matcher.IndexOf("𝠀A", "A", opts);
	AssertMatch(res, 4, 1);

	res = matcher.IndexOf("𞋀A", "A", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);

	res = matcher.IndexOf("ꡪaXCV", "XCV", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);

	res = matcher.IndexOf("aꡪXCV", "XCV", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);

	res = matcher.IndexOf("ꡪXCV", "XCV", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);

	res = matcher.IndexOf("!aXCV", "XCV", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);

	res = matcher.IndexOf("a!XCV", "XCV", opts);
	AssertMatch(res, 2, 3);

	res = matcher.IndexOf("G!a!XCV", "XCV", opts);
	AssertMatch(res, 4, 3);

	res = matcher.IndexOf("¶a!XCV", "XCV", opts);
	AssertMatch(res, 4, 3);

	res = matcher.IndexOf("¶!aXCV", "XCV", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);

	res = matcher.IndexOf("x\xC0¶XCV", "XCV", opts);
	AssertMatch(res, 4, 3);
}

void Test47()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { });

	auto res = matcher.IndexOf("B a", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B ", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B!fsd", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B¶", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B૰", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B𝠀", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B𞋀", "B", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);

	res = matcher.IndexOf("Bꡪa", "B", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);

	res = matcher.IndexOf("Baꡪ", "B", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);

	res = matcher.IndexOf("Bꡪ", "B", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);

	res = matcher.IndexOf("Ba!", "B", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);

	res = matcher.IndexOf("B!a", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B!a!G", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("B!a¶", "B", opts);
	AssertMatch(res, 0, 1);

	res = matcher.IndexOf("Ba!¶", "B", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
}

void Test48()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair("T", "7"));
	map.push_back(std::pair("H", "|-|"));
	map.push_back(std::pair("H", "н"));
	map.push_back(std::pair("I", "1"));
	map.push_back(std::pair("I", "|"));
	map.push_back(std::pair("N", "и"));
	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("N", "/v"));
	map.push_back(std::pair("N", "/V"));
	map.push_back(std::pair("G", "6"));
	map.push_back(std::pair("THI", "$"));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;

	auto matcher = ConfusableMatcher(map, { });

	auto in = "THING";

	opts.ContainsPosPointers = matcher.ComputeStringPosPointers(in);

	auto res = matcher.IndexOf("THERE IS NO SUCH THING", in, opts);
	AssertMatch(res, 17, 5);

	res = matcher.IndexOf("THERE IS NO SUCH T|-|ING", in, opts);
	AssertMatch(res, 17, 7);

	res = matcher.IndexOf("THERE IS NO SUCH 7н|/V6", in, opts);
	AssertMatch(res, 17, 7);

	res = matcher.IndexOf("THERE IS NO SUCH $/V6", in, opts);
	AssertMatch(res, 17, 4);
}

void Test49()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.StartIndex = 3;

	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("ee X", "X", opts);
	AssertMatch(res, 3, 1);

	res = matcher.IndexOf("ee aX", "X", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);
	ASSERT_EQUAL(4, res.Start);
	ASSERT_EQUAL(1, res.Size);

	res = matcher.IndexOf("ee Xa", "X", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
	ASSERT_EQUAL(3, res.Start);
	ASSERT_EQUAL(1, res.Size);

	res = matcher.IndexOf("ee a X", "X", opts);
	AssertMatch(res, 5, 1);

	res = matcher.IndexOf("ee X a", "X", opts);
	AssertMatch(res, 3, 1);

	res = matcher.IndexOf("ee X;duper", "X", opts);
	AssertMatch(res, 3, 1);

	res = matcher.IndexOf("ee yes\uFEFFX", "X", opts);
	AssertMatch(res, 9, 1);
}

void Test50()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.MatchRepeating = true;
	opts.StartIndex = 3;

	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("ee QQQ", "Q", opts);
	AssertMatch(res, 3, 3);

	res = matcher.IndexOf("ee aQQQ", "Q", opts);
	ASSERT(res.Status == WORD_BOUNDARY_FAIL_START || res.Status == WORD_BOUNDARY_FAIL_END);
	AssertMatchMulti(res, { 4, 4, 4, 5, 5, 6 }, { 1, 2, 3, 1, 2, 1 }, res.Status);

	res = matcher.IndexOf("ee QQQa", "Q", opts);

	ASSERT(res.Status == WORD_BOUNDARY_FAIL_START || res.Status == WORD_BOUNDARY_FAIL_END);
	AssertMatchMulti(res, { 3, 3, 3, 4, 4, 5 }, { 1, 2, 3, 1, 2, 1 }, res.Status);

	res = matcher.IndexOf("ee a QQQ", "Q", opts);
	AssertMatch(res, 5, 3);

	res = matcher.IndexOf("ee QQQ a", "Q", opts);
	AssertMatch(res, 3, 3);

	res = matcher.IndexOf("ee QQQ;duper", "Q", opts);
	AssertMatch(res, 3, 3);

	res = matcher.IndexOf("ee yes\u202FQQQ", "Q", opts);
	AssertMatch(res, 9, 3);
}

void Test51()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.StartIndex = 3;

	auto matcher = ConfusableMatcher(map, {});
	auto res = matcher.IndexOf("ee SUPER", "SUPER", opts);
	AssertMatch(res, 3, 5);

	res = matcher.IndexOf("ee aSUPER", "SUPER", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);
	ASSERT_EQUAL(4, res.Start);
	ASSERT_EQUAL(5, res.Size);

	res = matcher.IndexOf("ee SUPERa", "SUPER", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
	ASSERT_EQUAL(3, res.Start);
	ASSERT_EQUAL(5, res.Size);

	res = matcher.IndexOf("ee a SUPER", "SUPER", opts);
	AssertMatch(res, 5, 5);

	res = matcher.IndexOf("ee SUPER a", "SUPER", opts);
	AssertMatch(res, 3, 5);

	res = matcher.IndexOf("ee SUPER;duper", "SUPER", opts);
	AssertMatch(res, 3, 5);

	res = matcher.IndexOf("ee yes\u202FSUPER", "SUPER", opts);
	AssertMatch(res, 9, 5);
}

void Test52()
{
	std::vector<std::pair<std::string, std::string>> map;
	map.push_back(std::pair(" ", " "));

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.StartIndex = 3;

	auto matcher = ConfusableMatcher(map, { " " });
	auto res = matcher.IndexOf("ee a Q Q f", "Q Q", opts);
	AssertMatch(res, 5, 3);

	res = matcher.IndexOf("ee aQ Q f", "Q Q", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_START, res.Status);
	ASSERT_EQUAL(4, res.Start);
	ASSERT_EQUAL(3, res.Size);

	res = matcher.IndexOf("ee a Q Qf", "Q Q", opts);
	ASSERT_EQUAL(WORD_BOUNDARY_FAIL_END, res.Status);
	ASSERT_EQUAL(5, res.Start);
	ASSERT_EQUAL(3, res.Size);
}

void Test53()
{
	std::vector<std::pair<std::string, std::string>> map;

	CMOptions opts = { };
	opts.StatePushLimit = 50000;
	opts.MatchOnWordBoundary = true;
	opts.StartIndex = 3;

	auto matcher = ConfusableMatcher(map, { "\xff" });
}