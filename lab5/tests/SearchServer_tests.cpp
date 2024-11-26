#include "../searchServer/SearchServer.h"
#include "CoutBufferFixture.h"
#include "catch.hpp"
namespace fs = std::filesystem;

SCENARIO_METHOD(CoutBufferFixture, "Adding and searching documents") {
    GIVEN("A SearchServer with a few files") {
        SearchServer server;

        fs::path file1 = "file1.txt";
        fs::path file2 = "file2.txt";

        std::ofstream(file1) << "apple banana cherry";
        std::ofstream(file2) << "apple banana orange";

        server.AddFile(file1);
        server.AddFile(file2);

        WHEN("We search for a word that appears in both files") {
            std::string query = "Apple bAnAnA";

            THEN("We should get both files with the correct relevance") {
            	ClearOutput();
                server.Search(query);

                std::string output = GetOutput();
                REQUIRE(output.find("file1.txt") != std::string::npos);
                REQUIRE(output.find("file2.txt") != std::string::npos);
                REQUIRE(output.find("relevance:") != std::string::npos);
            }
        }

        WHEN("We search for a word that only appears in one file") {
            std::string query = "cherry";

            THEN("We should get the document that contains the word") {
            	ClearOutput();
                server.Search(query);

                std::string output = GetOutput();
                REQUIRE(output.find("file1.txt") != std::string::npos);
                REQUIRE(output.find("file2.txt") == std::string::npos);
            }
        }

        WHEN("We search for a word that does not exist in any file") {
            std::string query = "grape";

            THEN("We should get no results") {
            	ClearOutput();
                server.Search(query);

                std::string output = GetOutput();
                REQUIRE(output.find("id:") == std::string::npos);
            }
        }

        fs::remove(file1);
        fs::remove(file2);
    }
}

SCENARIO_METHOD(CoutBufferFixture, "Adding files from a directory") {
    GIVEN("A SearchServer and a directory with files") {
        SearchServer server;
        fs::path dir = "test_dir";
        
        create_directory(dir);
        std::ofstream(dir / "file1.txt") << "apple orange";
        std::ofstream(dir / "file2.txt") << "banana apple";

        server.AddDir(dir);

        WHEN("We search for a word in the directory") {
            std::string query = "apple";

            THEN("We should get results from both files") {
            	ClearOutput();
                server.Search(query);

                std::string output = GetOutput();
                REQUIRE(output.find("file1.txt") != std::string::npos);
                REQUIRE(output.find("file2.txt") != std::string::npos);
            }
        }

        remove_all(dir);
    }
}

SCENARIO_METHOD(CoutBufferFixture, "Adding files recursively from a directory") {
    GIVEN("A SearchServer and a directory with subdirectories and files") {
        SearchServer server;
        fs::path rootDir = "root_dir";
        fs::create_directory(rootDir);
        fs::create_directory(rootDir / "subdir");
        
        std::ofstream(rootDir / "file1.txt") << "apple banana";
        std::ofstream(rootDir / "subdir" / "file2.txt") << "banana orange";

        server.AddDirRecursive(rootDir);

        WHEN("We search for a word") {
            std::string query = "banana";

            THEN("We should get results from both files") {
            	ClearOutput();
                server.Search(query);

                std::string output = GetOutput();
                REQUIRE(output.find("file1.txt") != std::string::npos);
                REQUIRE(output.find("file2.txt") != std::string::npos);
            }
        }

        fs::remove_all(rootDir);
    }
}