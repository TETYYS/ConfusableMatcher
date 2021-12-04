#include "UnitTest.h"
#include <cute/cute_runner.h>
#include <cute/ide_listener.h>

#include "TestCases.cpp"

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
	s.push_back(CUTE(Test9));
	s.push_back(CUTE(Test10));
	s.push_back(CUTE(Test11));
	s.push_back(CUTE(Test12));
	s.push_back(CUTE(Test13));
	//s.push_back(CUTE(Test14));
	s.push_back(CUTE(Test15));
	s.push_back(CUTE(Test16));
	s.push_back(CUTE(Test17));
	s.push_back(CUTE(Test18));
	s.push_back(CUTE(Test19));
	s.push_back(CUTE(Test20));
	s.push_back(CUTE(Test21));
	s.push_back(CUTE(Test22));
	s.push_back(CUTE(Test23));
	s.push_back(CUTE(Test24));
	s.push_back(CUTE(Test25));
	s.push_back(CUTE(Test26));
	s.push_back(CUTE(Test27));
	s.push_back(CUTE(Test28));
	s.push_back(CUTE(Test29));
	s.push_back(CUTE(Test30));
	s.push_back(CUTE(Test31));
	s.push_back(CUTE(Test32));
	s.push_back(CUTE(Test33));
	s.push_back(CUTE(Test34));
	s.push_back(CUTE(Test35));
	s.push_back(CUTE(Test36));
	s.push_back(CUTE(Test37));
	s.push_back(CUTE(Test38));
	s.push_back(CUTE(Test39));
	s.push_back(CUTE(Test40));
	s.push_back(CUTE(Test41));
	s.push_back(CUTE(Test42));
	s.push_back(CUTE(Test43));
	s.push_back(CUTE(Test44));
	s.push_back(CUTE(Test45));
	s.push_back(CUTE(Test46));
	s.push_back(CUTE(Test47));
	s.push_back(CUTE(Test48));
	s.push_back(CUTE(Test49));
	s.push_back(CUTE(Test50));
	s.push_back(CUTE(Test51));
	s.push_back(CUTE(Test52));
	s.push_back(CUTE(Test53));
	s.push_back(CUTE(Test54));
	s.push_back(CUTE(Test55));
	s.push_back(CUTE(Test56));
	s.push_back(CUTE(Test57));
	s.push_back(CUTE(Test58));
	s.push_back(CUTE(Test59));
	s.push_back(CUTE(Test60));
	s.push_back(CUTE(Test61));
	s.push_back(CUTE(Test62));
	s.push_back(CUTE(Test63));
	cute::ide_listener<cute::null_listener> lis;
	return !cute::makeRunner(lis)(s, "suite");
}
