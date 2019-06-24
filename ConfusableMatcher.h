#pragma once

#include <codecvt>
#include <unordered_set>
#include <vector>

#ifdef WIN32
	#include <string_view>
	typedef std::string_view CMStringView;
#else
	#include <experimental/string_view>
	typedef std::experimental::string_view CMStringView;
#endif

#ifdef WIN32
	#define HASHCOMPARE_CLS stdext::hash_compare
#else
	#define HASHCOMPARE_CLS std::hash
#endif

namespace google
{
	template<class T>
		class libc_allocator_with_realloc;

	template<class Key, class T, class HashFcn, class EqualKey, class Alloc>
		class dense_hash_map;
};

struct MatchingState
{
	CMStringView In;
	CMStringView Contains;
	int StartingIndex;
	int MatchedChars;
	CMStringView LastMatched;

	MatchingState(CMStringView In, CMStringView Contains, int StartingIndex, int MatchedChars, CMStringView LastMatched)
	{
		this->In = In;
		this->Contains = Contains;
		this->StartingIndex = StartingIndex;
		this->MatchedChars = MatchedChars;
		this->LastMatched = LastMatched;
	}
};

constexpr int STACKVECTOR_SIZE = 16;
template<class T>
struct StackVector
{
	int CurSize = 0;
	bool IsStack = true;

public:
	T Stack[STACKVECTOR_SIZE];
	std::vector<T> Heap;

	void Push(T In)
	{
		if (IsStack) {
			if (CurSize + 1 > STACKVECTOR_SIZE) {
				IsStack = false;
				Heap = std::vector<T>(&Stack[0], &Stack[STACKVECTOR_SIZE - 1]);
				CurSize = 0;
			} else {
				Stack[CurSize++] = In;
				return;
			}
		}
		Heap.push_back(In);
	}

	void Reset()
	{
		if (IsStack)
			CurSize = 0;
		else
			Heap.clear();
	}

	size_t Size()
	{
		return IsStack ? CurSize : Heap.size();
	}
};

class ConfusableMatcher
{
	private:

	typedef google::dense_hash_map<
		char, // Value first char
		std::vector<std::string>*, // Values whole
		HASHCOMPARE_CLS<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, std::vector<std::string>*>>
	> CMInnerHashMap;

	google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				std::string, // Key whole
				CMInnerHashMap*
			>*
		>*,
		HASHCOMPARE_CLS<char>,
		std::equal_to<char>,
		google::libc_allocator_with_realloc<std::pair<const char, CMInnerHashMap>>
	>
		*TheMap;
	void
		GetMappings(CMStringView Key, CMStringView Value, StackVector<std::pair<std::string, std::string>> &Storage);
	std::pair<int, int>
		IndexOfFromView(CMStringView In, CMStringView Contains, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex);
	std::pair<int, int>
		IndexOfInner(MatchingState State, std::unordered_set<std::string> Skip, bool MatchRepeating);

	public:
	ConfusableMatcher(std::vector<std::pair<std::string, std::string>> InputMap, bool AddDefaultValues = true);
	bool
		AddMapping(std::string Key, std::string Value, bool CheckValueDuplicate);
	bool
		RemoveMapping(std::string Key, std::string Value);
	std::pair<int, int>
		IndexOf(std::string In, std::string Contains, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex = 0);

	~ConfusableMatcher();
};