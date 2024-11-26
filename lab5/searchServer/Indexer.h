#pragma once
#include "../threadPool/ThreadPool.h"
#include "InvertedIndex.h"
#include "StrToLower.h"
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>

class Indexer
{
public:
	Indexer(InvertedIndex& idx, size_t num_threads = std::thread::hardware_concurrency())
		: m_index(idx)
		, m_pool(num_threads)
	{
	}

	void AddFile(Path const& path)
	{
		if (m_indexedFiles.contains(path))
		{
			return;
		}
		m_indexedFiles.insert(path);
		m_pool.Dispatch([&, path]() {
			auto [words, wordCount] = ExtractWords(path);
			try
			{
				m_index.AddDocument(path, words, wordCount);
				std::cout << "Indexed file: " << path << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		});
	}

	void AddDirRecursive(const fs::path& dirPath)
	{
		for (const auto& entry : fs::recursive_directory_iterator(dirPath))
		{
			if (is_regular_file(entry))
			{
				AddFile(entry.path());
			}
		}
	}

	void AddDir(const fs::path& dirPath)
	{
		for (const auto& entry : fs::directory_iterator(dirPath))
		{
			if (is_regular_file(entry))
			{
				AddFile(entry.path());
			}
		}
	}

	void RemoveFile(const fs::path& path)
	{
		if (!m_indexedFiles.contains(path))
		{
			return;
		}
		m_indexedFiles.erase(path);
		m_pool.Dispatch([&, path]() {
			try
			{
				m_index.RemoveDocument(path);
				std::cout << "Removed file: " << path << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		});
	}

	void RemoveDirRecursive(const fs::path& dirPath)
	{
		for (const auto& entry : fs::recursive_directory_iterator(dirPath))
		{
			if (fs::is_regular_file(entry))
			{
				RemoveFile(entry.path());
			}
		}
	}

	void RemoveDir(const fs::path& dirPath)
	{
		for (const auto& entry : fs::directory_iterator(dirPath))
		{
			if (fs::is_regular_file(entry))
			{
				RemoveFile(entry.path());
			}
		}
	}

	void Wait()
	{
		m_pool.Wait();
	}

private:
	static std::pair<Words, size_t> ExtractWords(const fs::path& path)
	{
		Words words;
		size_t wordCount = 0;
		std::ifstream file(path);
		std::string word;
		while (file >> word)
		{
			++wordCount;
			++words[StrToLower(word)];
		}
		return { words, wordCount };
	}

private:
	InvertedIndex& m_index;
	std::unordered_set<Path> m_indexedFiles;
	ThreadPool m_pool;
};
