#pragma once
#include "Document.h"
#include "Path.h"
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
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
				for (const auto& word : it->second.words | std::views::keys)
				{
					m_index.at(word).erase(it->second.id);
					if (m_index.at(word).empty())
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

	std::vector<SearchResult> Search(const std::unordered_set<std::string>& query) const
	{
		std::shared_lock lock(m_indexMutex);
		std::unordered_map<uint64_t, double> docRelevance;

		for (const auto& word : query)
		{
			if (m_index.contains(word))
			{
				const auto& docsWithWord = m_index.at(word);
				const auto idf = std::log(m_documents.size() / docsWithWord.size());

				for (auto docId : docsWithWord)
				{
					const auto& [
						id,
						path,
						words,
						wordCount
					] = m_documents.find(docId)->second;
					const double occurrenceNum = words.at(word);
					const double tf = occurrenceNum / wordCount;

					docRelevance[docId] += tf * idf;
				}
			}
		}

		std::vector<SearchResult> result;
		result.reserve(docRelevance.size());
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

	Index m_index = {};
	uint64_t m_id = 0;
	std::unordered_map<uint64_t, Document> m_documents = {};
	mutable std::shared_mutex m_indexMutex;
};
