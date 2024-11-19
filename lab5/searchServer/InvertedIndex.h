#pragma once
#include "Document.h"
#include "Path.h"
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <cmath>
#include <ranges>

struct SearchResult
{
	uint64_t id;
	double relevance;
	Path path;
};

class InvertedIndex
{
public:
	uint64_t AddDocument(Path const& path, Words const& words, size_t wordCount)
	{
		std::unique_lock lock(m_indexMutex);

		auto id = ++m_id;
		m_documents[id] = Document(id, path, words, wordCount);

		for (const auto& word : std::views::keys(words))
		{
			m_index[word].insert(id);
		}

		return id;
	}

	bool RemoveDocument(const std::string& path)
	{
		std::unique_lock lock(m_indexMutex);
		for (auto it = m_documents.begin(); it != m_documents.end(); ++it)
		{
			if (it->second.path == path)
			{
				for (const auto& word : it->second.words)
				{
					m_index[word].erase(it->second.id);
					if (m_index[word].empty())
					{
						m_index.erase(word);
					}
				}
				m_documents.erase(it);
				return true;
			}
		}
		return false;
	}

	std::list<SearchResult> Search(const std::unordered_set<std::string>& query_words)
	{
		std::shared_lock lock(m_indexMutex);
		std::unordered_map<uint64_t, double> docRelevance;

		for (const auto& word : query_words)
		{
			if (m_index.find(word) != m_index.end())
			{
				const auto& docsWithWord = m_index[word];
				const auto idf = std::log(m_documents.size() / docsWithWord.size());

				for (auto docId : docsWithWord)
				{
					const auto& [
						id,
						path,
						words,
						wordCount
					] = m_documents.find(docId)->second;
					const auto occurrenceNum = words.at(word);
					const auto tf = occurrenceNum / wordCount;

					docRelevance[docId] += tf * idf;
				}
			}
		}

		std::list<SearchResult> result;
		for (const auto& [docId, relevance] : docRelevance)
		{
			result.push_back({
				docId,
				relevance,
				m_documents.at(docId).path,
			});
		}

		return result;
	}

private:
	using Index = std::unordered_map<std::string, std::unordered_set<uint64_t>>;
	using Documents = std::unordered_map<uint64_t, Document>;

	Index m_index;
	uint64_t m_id = 0;
	std::unordered_map<uint64_t, Document> m_documents;
	std::shared_mutex m_indexMutex;
};
