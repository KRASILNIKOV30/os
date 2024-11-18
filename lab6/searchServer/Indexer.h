#include <boost/asio.hpp>
#include <filesystem>
#include <future>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

class Indexer
{
private:
	InvertedIndex& index;
	boost::asio::thread_pool pool;

public:
	Indexer(InvertedIndex& idx, size_t num_threads = std::thread::hardware_concurrency())
		: index(idx)
		, pool(num_threads)
	{
	}

	void add_file(const fs::path& file_path)
	{
		boost::asio::post(pool, [this, file_path]() {
			std::unordered_set<std::string> words = extract_words(file_path);
			try
			{
				index.add_document(file_path.string(), words);
				std::cout << "Indexed file: " << file_path << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		});
	}

	void add_dir_recursive(const fs::path& dir_path)
	{
		for (const auto& entry : fs::recursive_directory_iterator(dir_path))
		{
			if (fs::is_regular_file(entry))
			{
				add_file(entry.path());
			}
		}
	}

	void wait_for_completion()
	{
		pool.join();
	}

private:
	std::unordered_set<std::string> extract_words(const fs::path& file_path)
	{
		std::unordered_set<std::string> words;
		std::ifstream file(file_path);
		std::string word;
		while (file >> word)
		{
			words.insert(word);
		}
		return words;
	}
};
