#include "UnitTests.h"
#include <cute/cute.h>
#include <ConfusableMatcher.h>
#include <cute/cute_runner.h>
#include <cute/ide_listener.h>

using namespace std;


void Test1()
{
	std::vector<std::tuple<char, std::string>> map;

	map.push_back(std::make_tuple('N', "T"));
	map.push_back(std::make_tuple('I', "E"));
	map.push_back(std::make_tuple('C', "S"));
	map.push_back(std::make_tuple('E', "T"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.StringContains("TEST", "NICE", { }, false, 0);
	ASSERT_EQUAL(std::get<0>(res), 0);
	ASSERT_EQUAL(std::get<1>(res), 4);
}

void Test2()
{
	std::vector<std::tuple<char, std::string>> map;

	map.push_back(std::make_tuple('V', "VA"));
	map.push_back(std::make_tuple('V', "VO"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.StringContains("VV", "VAVOVAVO", { }, false, 0);
	ASSERT_EQUAL(std::get<0>(res), -1);
	ASSERT_EQUAL(std::get<1>(res), -1);
	res = matcher.StringContains("VAVOVAVO", "VV", { }, false, 0);
	ASSERT_EQUAL(std::get<0>(res), 0);
	ASSERT_EQUAL(std::get<1>(res), 4);
	res = matcher.StringContains("VAVOVAVO", "VV", { }, false, 4);
	ASSERT_EQUAL(std::get<0>(res), 4);
	ASSERT_EQUAL(std::get<1>(res), 4);
	res = matcher.StringContains("VAVOVAVO", "VV", { }, false, 2);
	ASSERT_EQUAL(std::get<0>(res), 2);
	ASSERT_EQUAL(std::get<1>(res), 4);
	res = matcher.StringContains("VAVOVAVO", "VV", { }, false, 3);
	ASSERT_EQUAL(std::get<0>(res), 4);
	ASSERT_EQUAL(std::get<1>(res), 4);
}

void Test3()
{
	std::vector<std::tuple<char, std::string>> map;

	map.push_back(std::make_tuple('A', "\x02\x03"));
	map.push_back(std::make_tuple('B', "\xFA\xFF"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.StringContains("\x02\x03\xFA\xFF", "AB", { }, false, 0);
	ASSERT_EQUAL(std::get<0>(res), 0);
	ASSERT_EQUAL(std::get<1>(res), 4);
}

void Test4()
{
	std::vector<std::tuple<char, std::string>> map;

	map.push_back(std::make_tuple('S', "$"));
	map.push_back(std::make_tuple('D', "[)"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.StringContains("A__ _ $$$[)D", "ASD", { '_', ' ' }, true, 0);
	ASSERT_EQUAL(std::get<0>(res), 0);
	ASSERT_EQUAL(std::get<1>(res), 11);
}

void Test5()
{
	std::vector<std::tuple<char, std::string>> map;

	map.push_back(std::make_tuple('N', "/\\/"));
	map.push_back(std::make_tuple('N', "/\\"));
	map.push_back(std::make_tuple('I', "/"));

	auto matcher = ConfusableMatcher(map);
	auto res = matcher.StringContains("/\\/CE", "NICE", { }, false, 0);
	ASSERT_EQUAL(std::get<0>(res), 0);
	ASSERT_EQUAL(std::get<1>(res), 5);
}

int main()
{
	cute::suite s;
	s.push_back(CUTE(Test1));
	//s.push_back(CUTE(Test2));
	s.push_back(CUTE(Test3));
	//s.push_back(CUTE(Test4));
	s.push_back(CUTE(Test5));
	cute::ide_listener lis;
	return !cute::makeRunner(lis)(s, "suite");
}
