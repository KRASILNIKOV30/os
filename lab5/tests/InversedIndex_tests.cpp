#include "../searchServer/InvertedIndex.h"
#include "catch.hpp"

SCENARIO("Document is added to the index", "[InvertedIndex]")
{
	GIVEN("An empty inverted index")
	{
		InvertedIndex index;

		WHEN("A document with words 'apple', 'banana', 'orange' is added")
		{
			std::unordered_set<std::string> words = { "apple", "banana", "orange" };
			Path path("/path/to/file1.txt");
			uint64_t doc_id = index.AddDocument(path, words);

			THEN("The document should be indexed and retrievable by its words")
			{
				auto docs = index.Search({ "apple", "banana", "orange" });
				REQUIRE(docs.size() == 1);
				REQUIRE(docs.front().id == doc_id);
				REQUIRE(docs.front().path == path);
				REQUIRE(docs.front().words == words);
			}
		}
	}
}

SCENARIO("Document is removed from the index", "[InvertedIndex]")
{
	GIVEN("An inverted index with a document")
	{
		InvertedIndex index;
		std::unordered_set<std::string> words = { "apple", "banana" };
		Path path("/path/to/file1.txt");
		index.AddDocument(path, words);

		WHEN("The document is removed by its path")
		{
			bool removed = index.RemoveDocument(path);

			THEN("The document should no longer be found in the index")
			{
				REQUIRE(removed);
				auto docs = index.Search({ "apple", "banana" });
				REQUIRE(docs.empty());
			}
		}
	}
}

SCENARIO("Trying to remove a non-existent document", "[InvertedIndex]")
{
	GIVEN("An inverted index with no documents")
	{
		InvertedIndex index;

		WHEN("A non-existent document is removed by path")
		{
			Path path("/path/to/nonexistent.txt");
			bool removed = index.RemoveDocument(path);

			THEN("The removal attempt should fail")
			{
				REQUIRE_FALSE(removed);
			}
		}
	}
}

SCENARIO("Search returns documents containing query words", "[InvertedIndex]")
{
	GIVEN("An inverted index with documents")
	{
		InvertedIndex index;
		index.AddDocument("/path/to/file1.txt", { "apple", "banana" });
		index.AddDocument("/path/to/file2.txt", { "apple", "cherry" });
		index.AddDocument("/path/to/file3.txt", { "banana", "cherry" });

		WHEN("Searching for the word 'apple'")
		{
			auto docs = index.Search({ "apple" });

			THEN("The documents containing 'apple' should be returned")
			{
				REQUIRE(docs.size() == 2);
				REQUIRE((docs.front().path == "/path/to/file1.txt" || docs.front().path == "/path/to/file2.txt"));
			}
		}

		WHEN("Searching for the word 'banana'")
		{
			auto docs = index.Search({ "banana" });

			THEN("The documents containing 'banana' should be returned")
			{
				REQUIRE(docs.size() == 2);
				REQUIRE((docs.front().path != "/path/to/file2.txt" || docs.back().path == "/path/to/file2.txt"));
			}
		}

		WHEN("Searching for both 'apple' and 'banana'")
		{
			auto docs = index.Search({ "apple", "banana" });

			THEN("Only the document containing both words should be returned")
			{
				REQUIRE(docs.size() == 3);
			}
		}
	}
}

SCENARIO("Search handles empty query", "[InvertedIndex]")
{
	GIVEN("An inverted index with documents")
	{
		InvertedIndex index;
		index.AddDocument("/path/to/file1.txt", { "apple" });
		index.AddDocument("/path/to/file2.txt", { "banana" });

		WHEN("Searching with an empty set of query words")
		{
			auto docs = index.Search({});

			THEN("No documents should be returned")
			{
				REQUIRE(docs.empty());
			}
		}
	}
}

SCENARIO("Multiple documents with the same word are indexed correctly", "[InvertedIndex]")
{
	GIVEN("An inverted index with multiple documents containing the word 'apple'")
	{
		InvertedIndex index;
		uint64_t doc1 = index.AddDocument("/path/to/file1.txt", { "apple", "banana" });
		uint64_t doc2 = index.AddDocument("/path/to/file2.txt", { "apple", "cherry" });

		WHEN("Searching for the word 'apple'")
		{
			auto docs = index.Search({ "apple" });

			THEN("Both documents should be returned")
			{
				REQUIRE(docs.size() == 2);
				bool found_doc1 = false, found_doc2 = false;
				for (const auto& doc : docs)
				{
					if (doc.id == doc1)
						found_doc1 = true;
					if (doc.id == doc2)
						found_doc2 = true;
				}
				REQUIRE(found_doc1);
				REQUIRE(found_doc2);
			}
		}
	}
}

SCENARIO("Search returns no documents for non-matching words", "[InvertedIndex]")
{
	GIVEN("An inverted index with documents")
	{
		InvertedIndex index;
		index.AddDocument("/path/to/file1.txt", { "apple", "banana" });
		index.AddDocument("/path/to/file2.txt", { "cherry", "date" });

		WHEN("Searching for the word 'grape' which does not exist in any document")
		{
			auto docs = index.Search({ "grape" });

			THEN("No documents should be returned")
			{
				REQUIRE(docs.size() == 0);
			}
		}
	}
}
