#include "../searchServer/InvertedIndex.h"
#include "catch.hpp"

SCENARIO("AddDocument adds a new document to the index") {
    InvertedIndex index;

    GIVEN("A new document with a path and some words") {
        fs::path docPath = "doc1.txt";
        Words words = {{"apple", 3}, {"banana", 2}};
        size_t wordCount = 5;

        WHEN("The document is added to the index") {
            auto docId = index.AddDocument(docPath, words, wordCount);

            THEN("The document should be added with a valid id") {
                REQUIRE(docId > 0);
                REQUIRE(index.Search({"apple"}).size() == 1);
            }

            THEN("The word 'apple' should be indexed in the document") {
                auto results = index.Search({"apple"});
                REQUIRE(results.size() == 1);
                REQUIRE(results[0].id == docId);
                REQUIRE(results[0].path == docPath);
            }

            THEN("The word 'banana' should be indexed in the document") {
                auto results = index.Search({"banana"});
                REQUIRE(results.size() == 1);
                REQUIRE(results[0].id == docId);
                REQUIRE(results[0].path == docPath);
            }
        }
    }
}

SCENARIO("RemoveDocument removes a document from the index") {
    InvertedIndex index;

    GIVEN("A document is added to the index") {
        fs::path docPath = "doc1.txt";
        Words words = {{"apple", 3}, {"banana", 2}};
        size_t wordCount = 5;
        auto docId = index.AddDocument(docPath, words, wordCount);

        WHEN("The document is removed") {
            bool removed = index.RemoveDocument(docPath);

            THEN("The document should be removed from the index") {
                REQUIRE(removed);
                REQUIRE(index.Search({"apple"}).empty());
                REQUIRE(index.Search({"banana"}).empty());
            }

            THEN("Subsequent searches for this document should not return results") {
                auto results = index.Search({"apple"});
                REQUIRE(results.empty());
                results = index.Search({"banana"});
                REQUIRE(results.empty());
            }
        }
    }
}

SCENARIO("Search returns documents sorted by relevance") {
    InvertedIndex index;

    GIVEN("Multiple documents are added to the index") {
        fs::path docPath1 = "doc1.txt";
        Words words1 = {{"apple", 2}, {"banana", 1}, {"cat", 1}};
        size_t wordCount1 = 4;
        index.AddDocument(docPath1, words1, wordCount1);

        fs::path docPath2 = "doc2.txt";
        Words words2 = {{"apple", 1}, {"banana", 3}};
        size_t wordCount2 = 4;
        index.AddDocument(docPath2, words2, wordCount2);

        WHEN("A search is performed with a query") {
            auto results = index.Search({"apple", "banana", "cat"});

            THEN("The results should be sorted by relevance") {
                REQUIRE(results.size() == 2);
            }
        }
    }
}

SCENARIO("Search handles empty query gracefully") {
    InvertedIndex index;

    GIVEN("Multiple documents are added to the index") {
        fs::path docPath1 = "doc1.txt";
        Words words1 = {{"apple", 2}};
        size_t wordCount1 = 3;
        index.AddDocument(docPath1, words1, wordCount1);

        fs::path docPath2 = "doc2.txt";
        Words words2 = {{"banana", 3}};
        size_t wordCount2 = 4;
        index.AddDocument(docPath2, words2, wordCount2);

        WHEN("A search is performed with an empty query") {
            auto results = index.Search({});

            THEN("No documents should be returned") {
                REQUIRE(results.empty());
            }
        }
    }
}

SCENARIO("Search returns no results when no documents match the query") {
    InvertedIndex index;

    GIVEN("Multiple documents are added to the index") {
        fs::path docPath1 = "doc1.txt";
        Words words1 = {{"apple", 2}};
        size_t wordCount1 = 3;
        index.AddDocument(docPath1, words1, wordCount1);

        fs::path docPath2 = "doc2.txt";
        Words words2 = {{"banana", 3}};
        size_t wordCount2 = 4;
        index.AddDocument(docPath2, words2, wordCount2);

        WHEN("A search is performed with a query that matches no documents") {
            auto results = index.Search({"orange"});

            THEN("No results should be returned") {
                REQUIRE(results.empty());
            }
        }
    }
}
