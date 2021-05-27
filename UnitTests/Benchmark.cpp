#include <hayai.hpp>

class Tests { };

#include "TestCases.cpp"

/*BENCHMARK(Tests, Test1, 10, 1000)
{
	Test1();
}

BENCHMARK(Tests, Test2, 10, 1000)
{
	Test2();
}

BENCHMARK(Tests, Test3, 10, 1000)
{
	Test3();
}

BENCHMARK(Tests, Test4, 10, 1000)
{
	Test4();
}

BENCHMARK(Tests, Test5, 10, 1000)
{
	Test5();
}

BENCHMARK(Tests, Test6, 10, 1000)
{
	Test6();
}

BENCHMARK(Tests, Test7, 10, 1000)
{
	Test7();
}

BENCHMARK(Tests, LidlNormalizerTests, 10, 1000)
{
	LidlNormalizerTests();
}

BENCHMARK(Tests, Test8, 10, 1000)
{
	Test8();
}

BENCHMARK(Tests, Test9, 10, 1000)
{
	Test9();
}

BENCHMARK(Tests, Test10, 10, 1000)
{
	Test10();
}

BENCHMARK(Tests, Test11, 10, 1000)
{
	Test11();
}

BENCHMARK(Tests, Test12, 10, 1000)
{
	Test12();
}

BENCHMARK(Tests, Test13, 10, 1000)
{
	Test13();
}

BENCHMARK(Tests, Test15, 10, 1000)
{
	Test15();
}

BENCHMARK(Tests, Test16, 10, 1000)
{
	Test16();
}

BENCHMARK(Tests, Test17, 10, 1000)
{
	Test17();
}

BENCHMARK(Tests, Test18, 10, 1000)
{
	Test18();
}

BENCHMARK(Tests, Test19, 10, 1000)
{
	Test19();
}

BENCHMARK(Tests, Test20, 10, 1000)
{
	Test20();
}

BENCHMARK(Tests, Test21, 10, 1000)
{
	Test21();
}

BENCHMARK(Tests, Test22, 10, 1000)
{
	Test22();
}

BENCHMARK(Tests, Test23, 10, 1000)
{
	Test23();
}

BENCHMARK(Tests, Test24, 10, 1000)
{
	Test24();
}

BENCHMARK(Tests, Test25, 10, 1000)
{
	Test25();
}

BENCHMARK(Tests, Test26, 10, 1000)
{
	Test26();
}

BENCHMARK(Tests, Test27, 10, 1000)
{
	Test27();
}

BENCHMARK(Tests, Test28, 10, 1000)
{
	Test28();
}

BENCHMARK(Tests, Test29, 10, 1000)
{
	Test29();
}

BENCHMARK(Tests, Test30, 10, 1000)
{
	Test30();
}

BENCHMARK(Tests, Test31, 10, 1000)
{
	Test31();
}

BENCHMARK(Tests, Test32, 10, 1000)
{
	Test32();
}

BENCHMARK(Tests, Test33, 10, 1000)
{
	Test33();
}

BENCHMARK(Tests, Test34, 10, 1000)
{
	Test34();
}

BENCHMARK(Tests, Test35, 10, 1000)
{
	Test35();
}

BENCHMARK(Tests, Test36, 10, 1000)
{
	Test36();
}

BENCHMARK(Tests, Test37, 10, 1000)
{
	Test37();
}

BENCHMARK(Tests, Test38, 10, 1000)
{
	Test38();
}

BENCHMARK(Tests, Test39, 10, 1000)
{
	Test39();
}

BENCHMARK(Tests, Test40, 10, 1000)
{
	Test40();
}

BENCHMARK(Tests, Test41, 10, 1000)
{
	Test41();
}

BENCHMARK(Tests, Test42, 10, 1000)
{
	Test42();
}

BENCHMARK(Tests, Test43, 10, 1000)
{
	Test43();
}

BENCHMARK(Tests, Test44, 10, 1000)
{
	Test44();
}

BENCHMARK(Tests, Test45, 10, 1000)
{
	Test45();
}

void All()
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();
	Test7();
	LidlNormalizerTests();
	Test8();
	Test9();
	Test10();
	Test11();
	Test12();
	Test13();
	Test15();
	Test16();
	Test17();
	Test18();
	Test19();
	Test20();
	Test21();
	Test22();
	Test23();
	Test24();
	Test25();
	Test26();
	Test27();
	Test28();
	Test29();
	Test30();
	Test31();
	Test32();
	Test33();
	Test34();
	Test35();
	Test36();
	Test37();
	Test38();
	Test39();
	Test40();
	Test41();
	Test42();
	Test43();
	Test44();
	Test45();
}

BENCHMARK(Tests, All, 50, 1000)
{
	All();
}*/

class JsTest : public::hayai::Fixture
{
public:
	ConfusableMatcher *cm;
	CMOptions opts;

	virtual void SetUp()
	{
		std::vector<std::pair<std::string, std::string>> map;

		std::string spaces[] = {
			" ",
			"\u00A0",
			"᠎",
			" ",
			" ",
			" ",
			" ",
			" ",
			" ",
			" ",
			" ",
			" ",
			" ",
			" ",
			"​",
			" ",
			" ",
			"　"
		};

		for (auto space : spaces)
		{
			map.push_back(std::pair<std::string, std::string>(" ", space));
		}

		for (char lc = 'A';lc <= 'Z';lc++) {
			auto uc = lc + 0x20;
			map.push_back(std::pair<std::string, std::string>(std::string(1, lc), std::string(1, lc)));
			map.push_back(std::pair<std::string, std::string>(std::string(1, lc), std::string(1, uc)));
			map.push_back(std::pair<std::string, std::string>(std::string(1, uc), std::string(1, uc)));
			map.push_back(std::pair<std::string, std::string>(std::string(1, uc), std::string(1, lc)));
		}

		for (auto c = '0';c <= '9'; c++) {
			map.push_back(std::pair<std::string, std::string>(std::string(1, c), std::string(1, c)));
		}

		opts = { };
		opts.TimeoutNs = 1000000;
		opts.MatchOnWordBoundary = true;
		opts.MatchRepeating = true;

		this->cm = new ConfusableMatcher(map, { }, false);
	}

	virtual void TearDown()
	{
		delete this->cm;
	}

	void JsBench()
	{
		
	}
};

BENCHMARK_F(JsTest, IndexOf, 500, 10000)
{
	cm->IndexOf("SIMP", "SIMP", opts);
}

class LidlNormalizerTest : public::hayai::Fixture
{
public:
	ConfusableMatcher *matcher;
	CMOptions opts;
	std::vector<std::tuple<std::pair<std::string, std::string>, std::pair<int, int>>> data;

	virtual void SetUp()
	{
		matcher = LidlNormalizerCMSetup();
		data = LidlNormalizerData();
		opts = LidlNormalizerOpts();
	}

	virtual void TearDown()
	{
		delete matcher;
	}
};

BENCHMARK_F(LidlNormalizerTest, IndexOf, 10, 10000)
{
	for (auto entry : data) {
		std::pair<std::string, std::string> chr;
		std::pair<int, int> eq;
		std::tie(chr, eq) = entry;
		auto res = matcher->IndexOf(chr.first, chr.second, opts);
		AssertMatch(res, eq.first, eq.second);
	}
}

class WordBoundaryTest : public::hayai::Fixture
{
public:
	ConfusableMatcher *matcher;
	CMOptions opts;

	virtual void SetUp()
	{
		std::vector<std::pair<std::string, std::string>> map;

		opts = { };
		opts.TimeoutNs = 100000000;
		opts.MatchOnWordBoundary = true;
		opts.MatchRepeating = true;

		matcher = new ConfusableMatcher(map, { });
	}

	virtual void TearDown()
	{
		delete matcher;
	}
};

class PrecomputeLidlNormalizerTest : public::hayai::Fixture
{
public:
	ConfusableMatcher *matcher;
	std::vector<std::tuple<std::pair<std::string, std::string>, std::pair<int, int>>> data;
	std::vector<CMOptions> opts;

	virtual void SetUp()
	{
		matcher = LidlNormalizerCMSetup();
		data = LidlNormalizerData();

		for (auto entry : data) {
			std::pair<std::string, std::string> chr;
			std::pair<int, int> eq;
			std::tie(chr, eq) = entry;

			auto opt = LidlNormalizerOpts();
			opt.ContainsPosPointers = matcher->ComputeStringPosPointers(chr.second);
			opts.push_back(opt);
		}
	}

	virtual void TearDown()
	{
		delete matcher;
	}
};

BENCHMARK_F(PrecomputeLidlNormalizerTest, IndexOf, 10, 10000)
{
	for (auto x = 0;x < data.size();x++) {
		std::pair<std::string, std::string> chr;
		std::pair<int, int> eq;
		std::tie(chr, eq) = data[x];
		auto res = matcher->IndexOf(chr.first, chr.second, opts[x]);
		AssertMatch(res, eq.first, eq.second);
	}
}


class PrecomputeRealWorld : public::hayai::Fixture
{
public:
	ConfusableMatcher *matcher;
	std::string in = "AAAAAAAAASSAFSAFNFNFNISFNSIFSIFJSDFUDSHF ASUF/|/__/|/___%/|/%I%%/|//|/%%%%%NNNN/|/NN__/|/N__𝘪G___%____$__G__𝓰𝘦Ѓ";
	CMOptions opts;

	virtual void SetUp()
	{
		auto map = GetDefaultMap();

		opts = { };
		opts.TimeoutNs = 100000000;
		opts.MatchRepeating = true;
		matcher = new ConfusableMatcher(map, { "_", "%", "$" });
		opts.ContainsPosPointers = matcher->ComputeStringPosPointers("NIGGER");
	}

	virtual void TearDown()
	{
		delete matcher;
	}
};


BENCHMARK_F(PrecomputeRealWorld, IndexOf, 1, 1000000)
{
	auto res = matcher->IndexOf(in, "NIGGER", opts);

	AssertMatchMulti(res, { 64, 89 }, { 57, 32 });
}

class RealWorld : public::hayai::Fixture
{
public:
	ConfusableMatcher *matcher;
	std::string in = "AAAAAAAAASSAFSAFNFNFNISFNSIFSIFJSDFUDSHF ASUF/|/__/|/___%/|/%I%%/|//|/%%%%%NNNN/|/NN__/|/N__𝘪G___%____$__G__𝓰𝘦Ѓ";
	CMOptions opts;

	virtual void SetUp()
	{
		auto map = GetDefaultMap();

		opts = { };
		opts.TimeoutNs = 100000000;
		opts.MatchRepeating = true;
		matcher = new ConfusableMatcher(map, { "_", "%", "$" });
	}

	virtual void TearDown()
	{
		delete matcher;
	}
};

BENCHMARK_F(RealWorld, IndexOf, 1, 1000000)
{
	auto res = matcher->IndexOf(in, "NIGGER", opts);

	AssertMatchMulti(res, { 64, 89 }, { 57, 32 });
}