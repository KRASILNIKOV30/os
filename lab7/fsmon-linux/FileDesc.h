#pragma once
#include <filesystem>
#include <stdexcept>
#include <sys/inotify.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <system_error>
#include <unistd.h>
#include <utility>

namespace fs = std::filesystem;

class FileDesc
{
	constexpr static int InvalidDesc = -1;

public:
	FileDesc() = default;

	explicit FileDesc(const int desc)
		: m_desc(desc == InvalidDesc || desc >= 0
			         ? desc
			         : throw std::invalid_argument("Invalid file descriptor"))
	{
	}

	FileDesc(const FileDesc& other) = delete;
	FileDesc& operator=(const FileDesc& other) = delete;

	FileDesc(FileDesc&& other) noexcept
		: m_desc(std::exchange(other.m_desc, InvalidDesc))
	{
	}

	FileDesc& operator=(FileDesc&& other)
	{
		if (this != &other)
		{
			Swap(other);
			other.Close();
		}
		return *this;
	}

	void Swap(FileDesc& other) noexcept
	{
		std::swap(m_desc, other.m_desc);
	}

	~FileDesc()
	{
		try
		{
			Close();
		}
		catch (...)
		{
		}
	}

	[[nodiscard]] bool IsOpen() const noexcept
	{
		return m_desc != InvalidDesc;
	}

	void Close()
	{
		if (m_desc != InvalidDesc)
		{
			if (close(m_desc) != 0)
			{
				throw std::system_error(errno, std::generic_category());
			}
			m_desc = InvalidDesc;
		}
	}

	size_t Read(void* buffer, const size_t length)
	{
		EnsureOpen();
		if (const auto bytesRead = read(m_desc, buffer, length); bytesRead != -1)
		{
			return static_cast<size_t>(bytesRead);
		}
		throw std::system_error(errno, std::generic_category());
	}

	size_t Write(const void* buffer, const size_t length)
	{
		EnsureOpen();
		if (const auto bytesWritten = write(m_desc, buffer, length); bytesWritten != -1)
		{
			return static_cast<size_t>(bytesWritten);
		}
		throw std::system_error(errno, std::generic_category());
	}

	[[nodiscard]] size_t GetSize() const
	{
		struct stat file_info{};
		if (fstat(m_desc, &file_info) == -1)
		{
			throw std::system_error(errno, std::generic_category());
		}

		return file_info.st_size;
	}

	[[nodiscard]] unsigned char* MapFile() const
	{
		const auto mapPtr = static_cast<unsigned char*>(mmap(nullptr, GetSize(), PROT_READ, MAP_PRIVATE, m_desc, 0));
		if (mapPtr == MAP_FAILED)
		{
			throw std::system_error(errno, std::generic_category());
		}

		return mapPtr;
	}

	void UnmapFile(unsigned char* mapPtr) const
	{
		if (munmap(mapPtr, GetSize()) != 0)
		{
			throw std::system_error(errno, std::generic_category());
		}
	}

	[[nodiscard]] int AddWatcher(fs::path const& path, const uint32_t mask) const
	{
		return inotify_add_watch(m_desc, path.c_str(), mask);
	}

	void RemoveWatcher(const int wd) const noexcept
	{
		inotify_rm_watch(m_desc, wd);
	}

private:
	void EnsureOpen() const
	{
		if (m_desc == InvalidDesc)
		{
			throw std::logic_error("File descriptor is not open");
		}
	}

	int m_desc = InvalidDesc;
	unsigned char* m_mapPtr = nullptr;
};
