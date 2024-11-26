#pragma once
#include "../treadsafeSet/Timer.h"
#include "Indexer.h"
#include "InvertedIndex.h"

#include <algorithm>

constexpr int RESULT_MAX_SIZE = 10;

class SearchServer
{
public:
	SearchServer()
		: m_pool(std::thread::hardware_concurrency())
	{}

	void AddFile(Path const& path)
	{
		m_indexer.AddFile(path);
		m_indexer.Wait();
	}

	void AddDir(Path const& path)
	{
		m_indexer.AddDir(path);
		m_indexer.Wait();
	}

	void AddDirRecursive(Path const& path)
	{
		m_indexer.AddDirRecursive(path);
		m_indexer.Wait();
	}

	void RemoveFile(Path const& path)
	{
		m_indexer.RemoveFile(path);
		m_indexer.Wait();
	}

	void RemoveDir(Path const& path)
	{
		m_indexer.RemoveDir(path);
		m_indexer.Wait();
	}

	void RemoveDirRecursive(Path const& path)
	{
		m_indexer.RemoveDirRecursive(path);
		m_indexer.Wait();
	}

	void Search(std::string const& query) const
	{
		auto querySet = ExtractWords(query);
		auto result = MeasureTime(std::cout, "Search", &InvertedIndex::Search, m_index, querySet);
		std::ranges::sort(result, [](auto const& lhs, auto const& rhs) {
			return lhs.relevance > rhs.relevance;
		});

		for (int i = 0; i < result.size() && i < RESULT_MAX_SIZE; i++)
		{
			const auto doc = result[i];
			std::cout << "id: " << doc.id << ", relevance: " << doc.relevance << ", path: " << doc.path << std::endl;
		}
	}

	void SearchBatch(Path const& path)
	{
		std::ifstream file(path);
		std::string query;
		while (std::getline(file, query))
		{
			m_pool.Dispatch([&]() {
				std::cout << "query: " << query << std::endl;
				Search(query);
			});
		}
		m_pool.Wait();
	}

private:
	static std::unordered_set<std::string> ExtractWords(std::string const& str)
	{
		std::istringstream iss(str);
		std::unordered_set<std::string> words;
		std::string word;
		while (iss >> word)
		{
			words.insert(StrToLower(word));
		}

		return words;
	}

private:
	InvertedIndex m_index{};
	Indexer m_indexer = Indexer(m_index);
	ThreadPool m_pool;
};