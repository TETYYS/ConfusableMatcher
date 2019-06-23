#include "UnitTests.h"
#include <cute/cute.h>
#include <ConfusableMatcher.h>
#include <cute/cute_runner.h>
#include <cute/ide_listener.h>

using namespace std;


void Test1()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "T"));
	map.push_back(std::pair("I", "E"));
	map.push_back(std::pair("C", "S"));
	map.push_back(std::pair("E", "T"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf("TEST", "NICE", RECURSIVE, { }, false, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT_EQUAL(res.second, 4);
}

void Test2()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("V", "VA"));
	map.push_back(std::pair("V", "VO"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf("VV", "VAVOVAVO", RECURSIVE, { }, false, 0);
	ASSERT_EQUAL(res.first, -1);
	ASSERT_EQUAL(res.second, -1);
	res = matcher.IndexOf("VAVOVAVO", "VV", RECURSIVE, { }, false, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT(res.second == 3 || res.second == 4);
	res = matcher.IndexOf("VAVOVAVO", "VV", RECURSIVE, { }, false, 4);
	ASSERT_EQUAL(res.first, 4);
	ASSERT(res.second == 3 || res.second == 4);
	res = matcher.IndexOf("VAVOVAVO", "VV", RECURSIVE, { }, false, 2);
	ASSERT_EQUAL(res.first, 2);
	ASSERT(res.second == 3 || res.second == 4);
	res = matcher.IndexOf("VAVOVAVO", "VV", RECURSIVE, { }, false, 3);
	ASSERT_EQUAL(res.first, 4);
	ASSERT(res.second == 3 || res.second == 4);
}

void Test3()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("A", "\x02\x03"));
	map.push_back(std::pair("B", "\xFA\xFF"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf("\x02\x03\xFA\xFF", "AB", RECURSIVE, { }, false, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT_EQUAL(res.second, 4);
}

void Test4()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("S", "$"));
	map.push_back(std::pair("D", "[)"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf("A__ _ $$$[)D", "ASD", RECURSIVE, { "_", " " }, true, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT_EQUAL(res.second, 11);
}

void Test5()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("N", "/\\"));
	map.push_back(std::pair("I", "/"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf("/\\/CE", "NICE", RECURSIVE, { }, false, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT_EQUAL(res.second, 5);
}

void Test6()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "/\\/"));
	map.push_back(std::pair("V", "\\/"));
	map.push_back(std::pair("I", "/"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf("I/\\/AM", "INAN", RECURSIVE, { }, true, 0);
	ASSERT_EQUAL(res.first, -1);
	ASSERT_EQUAL(res.second, -1);
	res = matcher.IndexOf("I/\\/AM", "INAM", RECURSIVE, { }, true, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT_EQUAL(res.second, 6);
	res = matcher.IndexOf("I/\\/AM", "IIVAM", RECURSIVE, { }, true, 0);
	ASSERT_EQUAL(res.first, 0);
	ASSERT_EQUAL(res.second, 6);
}

std::vector<std::pair<std::string, std::string>> GetDefaultMap()
{
	std::vector<std::pair<std::string, std::string>> map;

	map.push_back(std::pair("N", "/[()[]]/"));
	map.push_back(std::pair("N", "\U000000f1"));
	map.push_back(std::pair("N", "|\\|"));
	map.push_back(std::pair("N", "\U00000245\U00000002f"));
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
	auto matcher = ConfusableMatcher(map);
	int iinc = 0;
	auto start = std::chrono::high_resolution_clock::now();
	std::pair<int, int> res;
	for (auto x = 0;x < 500000;x++) {
		res = matcher.IndexOf(in, "NIGGER", RECURSIVE, { "_", "%", "$" }, true, 0);
		iinc += res.first;
	}
	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> seconds = (finish - start) / 500000;

	std::ostringstream outp;
	outp << "Elapsed: " << seconds.count() << "s, iinc: " << iinc << std::endl;
	OutputDebugStringA(outp.str().data());

	ASSERT(
		(res.first == 64 && res.second == 57) ||
		(res.first == 89 && res.second == 32));

	start = std::chrono::high_resolution_clock::now();
	in = "AAAAAAAAASSAFSAFNFNFNISFNSIFSIFJSDFUDSHF ASUF/|/__/|/___%/|/%I%%/|//|/%%%%%NNNN/|/NN__NIGGER𝘪G___%____$__G__𝓰𝘦Ѓ";
	for (auto x = 0;x < 500000;x++) {
		auto res = in.find("NIGGER");
		iinc += res;
	}
	finish = std::chrono::high_resolution_clock::now();

	seconds = (finish - start) / 500000;

	std::ostringstream outp2;
	outp2 << "Elapsed: " << seconds.count() << "s, iinc: " << iinc << std::endl;
	OutputDebugStringA(outp2.str().data());
}

/*void Test7()
{
	auto map = GetDefaultMap();

	std::string in = "AAAAAAAAASSAFSAFNFNFNISFNSIFSIFJSDFUDSHF ASUF/|/__/|/___%/|/%I%%/|//|/%%%%%NNNN/|/NN__/|/N__𝘪G___%____$__G__𝓰𝘦Ѓ";
	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf(in, "NIGGER", RECURSIVE, { "_", "%", "$" }, true, 0);

	ASSERT(
		(res.first == 64 && res.second == 57) ||
		(res.first == 89 && res.second == 32));
}*/

void LidlNormalizerTests()
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
		"U", "A", " ", "S", "M", "O", "L", "N", "A", "T", "I", "O", "N", "N", "I", "G", "N", "I"
	};
	std::vector<std::string> vals = {
		"\U00000105", "\U0000ab31", "\U00001d43", "\U000000e5", "\U0000249d", "\U000000fc", "\U000000dc", "\U000000f6", "\U000000d6", "\U000000e4", "\U000000c4",
		"\U0000249c", "\U0000249e", "\U0000249f", "\U000024a0", "\U000024a1", "\U000024a2", "\U000024a3", "\U000024a4", "\U000024a5", "\U000024a6", "\U000024a7", "\U000024a8", "\U000024a9", "\U000024aa", "\U000024ab", "\U000024ac", "\U000024ad", "\U000024ae", "\U000024af", "\U000024b0", "\U000024b1", "\U000024b2", "\U000024b3", "\U000024b4", "\U000024cf", "\U000024d0", "\U000024d1", "\U000024d2", "\U000024d3", "\U000024d4", "\U000024d5", "\U000024d6", "\U000024d7", "\U000024d8", "\U000024d9", "\U000024da", "\U000024db", "\U000024dc", "\U000024dd", "\U000024de", "\U000024df", "\U000024e0", "\U000024e1", "\U000024e2", "\U000024e3", "\U000024e4", "\U000024e5", "\U000024e6", "\U000024e7", "\U000024e8", "\U000024e9", "\U000024ea", "\U0001d552", "\U0001d553", "\U0001d554", "\U0001d555", "\U0001d556", "\U0001d557", "\U0001d558", "\U0001d559", "\U0001d55a", "\U0001d55b", "\U0001d55c", "\U0001d55d", "\U0001d55e", "\U0001d55f", "\U0001d560", "\U0001d561", "\U0001d562", "\U0001d563", "\U0001d564", "\U0001d565", "\U0001d566", "\U0001d567", "\U0001d568", "\U0001d569", "\U0001d56a", "\U0001d56b", "\U0001f130", "\U0001f131", "\U0001f132", "\U0001f133", "\U0001f134", "\U0001f135", "\U0001f136", "\U0001f137", "\U0001f138", "\U0001f139", "\U0001f13a", "\U0001f13b", "\U0001f13c", "\U0001f13d", "\U0001f13e", "\U0001f13f", "\U0001f140", "\U0001f141", "\U0001f142", "\U0001f143", "\U0001f144", "\U0001f145", "\U0001f146", "\U0001f147", "\U0001f148", "\U0001f149", "\U000020b3", "\U00000e3f", "\U000020b5", "\U00000110", "\U00000246", "\U000020a3", "\U000020b2", "\U00002c67", "\U00000142", "\U0000004a", "\U000020ad", "\U00002c60", "\U000020a5", "\U000020a6", "\U000000d8", "\U000020b1", "\U00000051", "\U00002c64", "\U000020b4", "\U000020ae", "\U00000244", "\U00000056", "\U000020a9", "\U000004fe", "\U0000024e", "\U00002c6b", "\U0001d586", "\U0001d587", "\U0001d588", "\U0001d589", "\U0001d58a", "\U0001d58b", "\U0001d58c", "\U0001d58d", "\U0001d58e", "\U0001d58f", "\U0001d590", "\U0001d591", "\U0001d592", "\U0001d593", "\U0001d594", "\U0001d595", "\U0001d596", "\U0001d597", "\U0001d598", "\U0001d599", "\U0001d59a", "\U0001d59b", "\U0001d59c", "\U0001d59d", "\U0001d59e", "\U0001d59f", "\U0001f170", "\U0001f171", "\U0001f172", "\U0001f173", "\U0001f174", "\U0001f175", "\U0001f176", "\U0001f177", "\U0001f178", "\U0001f179", "\U0001f17a", "\U0001f17b", "\U0001f17c", "\U0001f17d", "\U0001f17e", "\U0001f17f", "\U0001f180", "\U0001f181", "\U0001f182", "\U0001f183", "\U0001f184", "\U0001f185", "\U0001f186", "\U0001f187", "\U0001f188", "\U0001f189", "\U0001f1fa", "\U0001f1e6", " ", "\U000002e2", "\U00001d50", "\U00001d52", "\U000002e1", "\U0000207f", "\U00001d43", "\U00001d57", "\U00001da6", "\U00001d52", "\U0000207f", "\U0000041d", "\U00000438", "\U00000433", "\U0001F1F3", "\U0001F1EE"
	};
	
	for (auto x = 0;x < keys.size();x++)
		map.push_back(std::pair(keys[x], vals[x]));

	auto matcher = ConfusableMatcher(map);

	auto data = {
		std::make_tuple(std::vector({ "\U00000105", "A" }), std::vector({ 0, 2 })),
		std::make_tuple(std::vector({ "\U0000ab31", "A" }), std::vector({ 0, 3 })),
		std::make_tuple(std::vector({ "\U00001d43", "A" }), std::vector({ 0, 3 })),
		std::make_tuple(std::vector({ "abc \U000000e5 def", "ABC A DEF" }), std::vector({ 0, 10 })),
		std::make_tuple(std::vector({ "\U000002e2\U00001d50\U00001d52\U000002e1 \U0000207f\U00001d43\U00001d57\U00001da6\U00001d52\U0000207f", "SMOL NATION" }), std::vector({ 0, 29 })),
		std::make_tuple(std::vector({ "\U0000041d\U00000438\U00000433", "NIG" }), std::vector({ 0, 6 })),
		std::make_tuple(std::vector({ "\U0001f1fa\U0001f1e6XD", "UAXD" }), std::vector({ 0, 10 })),
		//std::make_tuple(std::vector({ "🆓 ICE", "FREE ICE" }), std::vector({ 0, 5 })),
		std::make_tuple(std::vector({ "chocolate \U0001F1F3\U0001F1EEb", "CHOCOLATE NIB" }), std::vector({ 0, 19 })),
		std::make_tuple(std::vector({ "\U0001f171lueberry", "BLUEBERRY" }), std::vector({ 0, 12 })),
		std::make_tuple(std::vector({ "\U0000249d", "B" }), std::vector({ 0, 3 })),
		std::make_tuple(std::vector({ "\U000000fc \U000000dc \U000000f6 \U000000d6 \U000000e4 \U000000c4", "U U O O A A" }), std::vector({ 0, 17 })),
		//std::make_tuple(std::vector({ "ᴭ", "AE" }), std::vector({ 0, 1 })),
		std::make_tuple(std::vector({ "\U0000249c \U0000249d \U0000249e \U0000249f \U000024a0 \U000024a1 \U000024a2 \U000024a3 \U000024a4 \U000024a5 \U000024a6 \U000024a7 \U000024a8 \U000024a9 \U000024aa \U000024ab \U000024ac \U000024ad \U000024ae \U000024af \U000024b0 \U000024b1 \U000024b2 \U000024b3 \U000024b4", "A B C D E F G H I J K L M N O P Q R S T U V W X Y" }), std::vector({ 0, 99 })),
		std::make_tuple(std::vector({ "\U000024cf\U000024d0\U000024d1\U000024d2\U000024d3\U000024d4\U000024d5\U000024d6\U000024d7\U000024d8\U000024d9\U000024da\U000024db\U000024dc\U000024dd\U000024de\U000024df\U000024e0\U000024e1\U000024e2\U000024e3\U000024e4\U000024e5\U000024e6\U000024e7\U000024e8\U000024e9\U000024ea", "ZABCDEFGHIJKLMNOPQRSTUVWXYZ0" }), std::vector({ 0, 84 })),
		std::make_tuple(std::vector({ "\U0001d552\U0001d553\U0001d554\U0001d555\U0001d556\U0001d557\U0001d558\U0001d559\U0001d55a\U0001d55b\U0001d55c\U0001d55d\U0001d55e\U0001d55f\U0001d560\U0001d561\U0001d562\U0001d563\U0001d564\U0001d565\U0001d566\U0001d567\U0001d568\U0001d569\U0001d56a\U0001d56b", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" }), std::vector({ 0, 104 })),
		std::make_tuple(std::vector({ "\U0001f130\U0001f131\U0001f132\U0001f133\U0001f134\U0001f135\U0001f136\U0001f137\U0001f138\U0001f139\U0001f13a\U0001f13b\U0001f13c\U0001f13d\U0001f13e\U0001f13f\U0001f140\U0001f141\U0001f142\U0001f143\U0001f144\U0001f145\U0001f146\U0001f147\U0001f148\U0001f149", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" }), std::vector({ 0, 104 })),
		std::make_tuple(std::vector({ "\U000020b3\U00000e3f\U000020b5\U00000110\U00000246\U000020a3\U000020b2\U00002c67\U00000142J\U000020ad\U00002c60\U000020a5\U000020a6\U000000d8\U000020b1Q\U00002c64\U000020b4\U000020ae\U00000244V\U000020a9\U000004fe\U0000024e\U00002c6b", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" }), std::vector({ 0, 65 })),
		std::make_tuple(std::vector({ "\U0001d586\U0001d587\U0001d588\U0001d589\U0001d58a\U0001d58b\U0001d58c\U0001d58d\U0001d58e\U0001d58f\U0001d590\U0001d591\U0001d592\U0001d593\U0001d594\U0001d595\U0001d596\U0001d597\U0001d598\U0001d599\U0001d59a\U0001d59b\U0001d59c\U0001d59d\U0001d59e\U0001d59f", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" }), std::vector({ 0, 104 })),
		std::make_tuple(std::vector({ "\U0001f170\U0001f171\U0001f172\U0001f173\U0001f174\U0001f175\U0001f176\U0001f177\U0001f178\U0001f179\U0001f17a\U0001f17b\U0001f17c\U0001f17d\U0001f17e\U0001f17f\U0001f180\U0001f181\U0001f182\U0001f183\U0001f184\U0001f185\U0001f186\U0001f187\U0001f188\U0001f189", "ABCDEFGHIJKLMNOPQRSTUVWXYZ" }), std::vector({ 0, 104 }))
	};

	for (auto entry : data) {
		std::vector<const char*> chr;
		std::vector<int> eq;
		std::tie(chr, eq) = entry;
		auto res = matcher.IndexOf(chr[0], chr[1], RECURSIVE, { }, true, 0);
		ASSERT_EQUAL(res.first, eq[0]);
		ASSERT_EQUAL(res.second, eq[1]);
	}
}

void Test8()
{
	std::vector<std::pair<std::string, std::string>> map;

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.IndexOf(
		"[̲̅a̲̅][̲̅b̲̅][̲̅c̲̅][̲̅d̲̅][̲̅e̲̅][̲̅f̲̅][̲̅g̲̅][̲̅h̲̅][̲̅i̲̅][̲̅j̲̅][̲̅k̲̅][̲̅l̲̅][̲̅m̲̅][̲̅n̲̅][̲̅o̲̅][̲̅p̲̅][̲̅q̲̅][̲̅r̲̅][̲̅s̲̅][̲̅t̲̅][̲̅u̲̅][̲̅v̲̅][̲̅w̲̅][̲̅x̲̅][̲̅y̲̅][̲̅z̲̅][̲̅0̲̅][̲̅1̲̅][̲̅2̲̅][̲̅3̲̅][̲̅4̲̅][̲̅5̲̅][̲̅6̲̅][̲̅7̲̅][̲̅8̲̅][̲̅9̲̅][̲̅0̲̅]",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890",
		RECURSIVE,
		{ "\U00000332", "\U00000305", "[", "]" },
		false,
		0);
	ASSERT_EQUAL(res.first, 5);
	ASSERT_EQUAL(res.second, 397);
}

int main()
{
	cute::suite s;
	s.push_back(CUTE(Test1));
	s.push_back(CUTE(Test2));
	s.push_back(CUTE(Test3));
	s.push_back(CUTE(Test4));
	s.push_back(CUTE(Test5));
	s.push_back(CUTE(Test6));
	s.push_back(CUTE(Test7));
	s.push_back(CUTE(LidlNormalizerTests));
	s.push_back(CUTE(Test8));
	cute::ide_listener lis;
	return !cute::makeRunner(lis)(s, "suite");
}
