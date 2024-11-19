#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "FileDesc.h"
#include "ThreadsafeVector.h"
#include "Timer.h"

constexpr int CHARS_NUMBER = 256;

using HistogramType = std::array<size_t, CHARS_NUMBER>;
using Hists = ThreadsafeVector<HistogramType>;

void CountByteFrequencies(const unsigned char* start, const unsigned char* end, Hists& result)
{
	HistogramType histogram;
	for (const unsigned char* ptr = start; ptr < end; ++ptr)
	{
		++histogram[*ptr];
	}
	result.PushBack(histogram);
}

void UnionHists(Hists const& histograms, HistogramType& result)
{
	for (const auto& hist : histograms)
	{
		for (int i = 0; i < CHARS_NUMBER; ++i)
		{
			result.at(i) += hist.at(i);
		}
	}
}

void CreateHistogram(HistogramType& result, unsigned char* mapPtr, const size_t fileSize, const int threadsNum)
{
	Hists histograms;
	{
		const size_t chunkSize = fileSize / threadsNum;
		std::vector<std::jthread> threads;

		int i = 0;
		for (; i < threadsNum; ++i)
		{
			const size_t start = i * chunkSize;
			const size_t end = i == threadsNum - 1 ? fileSize : (i + 1) * chunkSize;

			threads.emplace_back(CountByteFrequencies, mapPtr + start, mapPtr + end, std::ref(histograms));
		}
	}

	UnionHists(histograms, result);
}

void Histogram(std::string const& filename, const int threadsNum)
{
	const FileDesc file(open(filename.c_str(), O_RDONLY));
	const auto fileSize = file.GetSize();
	const auto mapPtr = file.MapFile();

	HistogramType histogram{};
	CreateHistogram(histogram, mapPtr, fileSize, threadsNum);

	for (int i = 0; i < CHARS_NUMBER; ++i)
	{
		std::cout << histogram[i] << std::endl;
	}

	file.UnmapFile(mapPtr);
}

int main(const int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " FILE_NAME NUM_THREADS" << std::endl;
		return 1;
	}

	const std::string filename = argv[1];
	const int threadsNum = std::stoi(argv[2]);

	MeasureTime(std::cout, "histogram", Histogram, filename, threadsNum);

	return EXIT_SUCCESS;
}
