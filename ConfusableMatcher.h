#pragma once

#include <codecvt>
#include <string_view>
#include <unordered_set>
#include <iterator>

namespace google
{
	template<class T>
		class libc_allocator_with_realloc;

	template<class Key, class T, class HashFcn = stdext::hash_compare<Key>,
		class EqualKey = std::equal_to<Key>,
		class Alloc = libc_allocator_with_realloc<std::pair<const Key, T>>>
		class dense_hash_map;
};

struct MatchingState
{
	std::string_view In;
	std::string_view Contains;
	int StartingIndex;
	int MatchedChars;
	std::string_view LastMatched;

	MatchingState(std::string_view In, std::string_view Contains, int StartingIndex, int MatchedChars, std::string_view LastMatched)
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
	int CurSize;
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
	google::dense_hash_map<
		char, // Key first char
		std::vector<
			std::pair<
				std::string, // Key whole
				google::dense_hash_map<
					char, // Value first char
					std::vector<std::string>* // Values whole
				>*
			>*
		>*
	>
		*TheMap;
	void
		GetMappings(std::string_view Key, std::string_view Value, StackVector<std::pair<std::string, std::string>> &Storage);
	std::pair<int, int>
		IndexOfFromView(std::string_view In, std::string_view Contains, std::unordered_set<std::string> Skip, bool MatchRepeating, int StartIndex);
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