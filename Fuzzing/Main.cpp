#include <ConfusableMatcher.h>
#include "../UnitTests/TestCases.cpp"

int main(int argc, char **argv)
{
	std::string in;
	std::string contains;
	std::unordered_set<std::string> skip;

	bool precompute;
	bool wordBoundary;
	bool matchRepeating;
	int startIndex;
	bool startFromEnd;

	try {

	auto x = 0;

	for (std::string line; std::getline(std::cin, line);) {
		size_t last = 0;
		size_t next = 0;

		switch (x) {
			case 0:
				in = line;
				break;
			case 1:
				contains = line;
				break;
			case 2:
				while ((next = line.find('|', last)) != std::string::npos) {
					skip.emplace(line.substr(last, next-last));
					last = next + 1;
				}
				skip.emplace(line.substr(last));

				break;
			case 3:
				precompute = line.size() == 1 && line[0] == '1';
				break;
			case 4:
				wordBoundary = line.size() == 1 && line[0] == '1';
				break;
			case 5:
				matchRepeating = line.size() == 1 && line[0] == '1';
				break;
			case 6:
				startIndex = atoi(line.c_str());
				break;
			case 7:
				startFromEnd = line.size() == 1 && line[0] == '1';
				break;
			case 8:
				goto a;
		}
		x++;
	}

a:;

	} catch (std::exception e) {
		return 0;
	}

	std::vector<std::pair<std::string, std::string>> map = GetDefaultMap();

	auto cm = ConfusableMatcher(map, skip, true);

	CMOptions opts = { };
	opts.MatchRepeating = matchRepeating;
	opts.MatchOnWordBoundary = wordBoundary;
	if (precompute) {
		opts.ContainsPosPointers = cm.ComputeStringPosPointers(contains);
	}
	opts.StartIndex = startIndex;
	opts.StartFromEnd = startFromEnd;
	opts.StatePushLimit = 100000;

	cm.IndexOf(in, contains, opts);
}