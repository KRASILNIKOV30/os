#include <iostream>
#include <fstream>
#include <vector>
#include <atomic>
#include <thread>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void CountByteFrequencies(const unsigned char* start, const unsigned char* end, std::atomic<int>* result)
{
	for (const unsigned char* ptr = start; ptr < end; ++ptr)
	{
		++result[*ptr];
	}
}

void Histogram(const char* filename, int num_threads)
{
	// Открываем файл
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		std::cerr << "Error opening file " << filename << std::endl;
		return;
	}

	// Получаем размер файла
	struct stat file_info{};
	if (fstat(fd, &file_info) == -1)
	{
		std::cerr << "Error getting file size" << std::endl;
		close(fd);
		return;
	}
	size_t file_size = file_info.st_size;

	// Отображаем файл в память
	unsigned char* mmap_ptr = (unsigned char*)mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mmap_ptr == MAP_FAILED)
	{
		std::cerr << "Error mapping file to memory" << std::endl;
		close(fd);
		return;
	}

	// Массив для хранения частот встречаемости байтов (атомарный для потоков)
	std::atomic<int> result[256] = { 0 };

	// Разделяем работу между потоками
	size_t chunk_size = file_size / num_threads;
	std::vector<std::jthread> threads;

	for (int i = 0; i < num_threads; ++i)
	{
		size_t start = i * chunk_size;
		size_t end = (i == num_threads - 1) ? file_size : (i + 1) * chunk_size;

		// Используем std::jthread, который автоматически завершит поток
		threads.emplace_back(CountByteFrequencies, mmap_ptr + start, mmap_ptr + end, result);
	}

	// Ожидаем завершения всех потоков (не нужно вручную вызывать join, это сделает jthread)
	// std::jthread автоматически ожидает завершения.

	// Выводим результаты
	for (int i = 0; i < 256; ++i)
	{
		std::cout << result[i] << std::endl;
	}

	// Освобождаем память и закрываем файл
	munmap(mmap_ptr, file_size);
	close(fd);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " FILE_NAME NUM_THREADS" << std::endl;
		return 1;
	}

	const char* filename = argv[1];
	int num_threads = std::stoi(argv[2]);

	Histogram(filename, num_threads);

	return 0;
}
