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
		opts.StatePushLimit = 50000;
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

BENCHMARK_F(JsTest, IndexOf, 1000, 10000)
{
	cm->IndexOf("SIMP", "SIMP", opts);
}

class LidlNormalizerTest : public::hayai::Fixture
{
public:
	ConfusableMatcher *matcher;
	CMOptions opts;
	std::vector<std::tuple<std::vector<std::string>, std::vector<int>>> data;

	virtual void SetUp()
	{
		matcher = LidlNormalizerCMSetup();
		data = LidlNormalizerData();
		opts = LidlNormalizerOpts();
	}

	virtual void TearDown()
	{
	}
};

BENCHMARK_F(LidlNormalizerTest, IndexOf, 30, 10000)
{
	for (auto entry : data) {
		std::vector<std::string> chr;
		std::vector<int> eq;
		std::tie(chr, eq) = entry;
		auto res = matcher->IndexOf(chr[0], chr[1], opts);
		AssertMatch(res, eq[0], eq[1]);
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
		opts.StatePushLimit = 50000;
		opts.MatchOnWordBoundary = true;
		opts.MatchRepeating = true;

		matcher = new ConfusableMatcher(map, { });
	}

	virtual void TearDown()
	{
	}
};

BENCHMARK_F(WordBoundaryTest, MatchWordBoundaryToRight, 10000, 100000)
{
	ASSERT(matcher->MatchWordBoundaryToRight("!"));
	ASSERT(matcher->MatchWordBoundaryToRight("fsd!"));
	ASSERT(matcher->MatchWordBoundaryToRight("¶"));
	ASSERT(matcher->MatchWordBoundaryToRight("૰"));
	ASSERT(matcher->MatchWordBoundaryToRight("𝠀"));
	ASSERT(!matcher->MatchWordBoundaryToRight("𞋀"));
	ASSERT(!matcher->MatchWordBoundaryToRight("!a"));
	ASSERT(matcher->MatchWordBoundaryToRight("a!"));
	ASSERT(matcher->MatchWordBoundaryToRight("G!a!"));
	ASSERT(matcher->MatchWordBoundaryToRight("¶a!"));
	ASSERT(!matcher->MatchWordBoundaryToRight("¶!a"));
}

BENCHMARK_F(WordBoundaryTest, MatchWordBoundaryToLeft, 10000, 100000)
{
	ASSERT(matcher->MatchWordBoundaryToLeft(" a"));
	ASSERT(matcher->MatchWordBoundaryToLeft(" "));
	ASSERT(matcher->MatchWordBoundaryToLeft("!fsd"));
	ASSERT(matcher->MatchWordBoundaryToLeft("¶"));
	ASSERT(matcher->MatchWordBoundaryToLeft("૰"));
	ASSERT(matcher->MatchWordBoundaryToLeft("𝠀"));
	ASSERT(!matcher->MatchWordBoundaryToLeft("𞋀"));
	ASSERT(!matcher->MatchWordBoundaryToLeft("ꡪa"));
	ASSERT(!matcher->MatchWordBoundaryToLeft("aꡪ"));
	ASSERT(!matcher->MatchWordBoundaryToLeft("ꡪ"));
	ASSERT(!matcher->MatchWordBoundaryToLeft("a!"));
	ASSERT(matcher->MatchWordBoundaryToLeft("!a"));
	ASSERT(matcher->MatchWordBoundaryToLeft("!a!G"));
	ASSERT(matcher->MatchWordBoundaryToLeft("!a¶"));
	ASSERT(!matcher->MatchWordBoundaryToLeft("a!¶"));
}