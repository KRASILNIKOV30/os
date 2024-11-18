#pragma once
#include "Document.h"
#include "Path.h"
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>
#include <list>

class InvertedIndex
{
public:
	uint64_t AddDocument(Path const& path, const std::unordered_set<std::string>& words)
	{
		std::unique_lock lock(m_indexMutex);

        auto id = ++m_id;
		documents[id] = Document(id, path, words);

		for (const auto& word : words)
		{
			m_index[word].insert(id);
		}

		return id;
	}

	bool RemoveDocument(const std::string& path)
	{
		std::unique_lock lock(m_indexMutex);
		for (auto it = documents.begin(); it != documents.end(); ++it)
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
				documents.erase(it);
				return true;
			}
		}
		return false;
	}

	std::list<Document> Search(const std::unordered_set<std::string>& query_words)
	{
		std::shared_lock lock(m_indexMutex);
		std::unordered_map<uint64_t, float> doc_relevance;

		for (const auto& word : query_words)
		{
			if (m_index.find(word) != m_index.end())
			{
				for (auto doc_id : m_index[word])
				{
					// Считаем частоту термина для каждого документа
					float tf = 1.0f; // Для простоты здесь берем все слова с одинаковым TF
					doc_relevance[doc_id] += tf; // В дальнейшем можно использовать TF-IDF
				}
			}
		}

		std::list<Document> result;
		for (const auto& [doc_id, relevance] : doc_relevance)
		{
			result.push_back(documents[doc_id]);
		}

		return result;
	}

private:
	using Index = std::unordered_map<std::string, std::unordered_set<uint64_t>>;
	using Documents = std::unordered_map<uint64_t, Document>;

	Index m_index;
	uint64_t m_id = 0;
	std::unordered_map<uint64_t, Document> documents;
	std::shared_mutex m_indexMutex;
};
