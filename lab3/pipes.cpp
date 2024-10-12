#include <iostream>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include <utility>
#include <vector>

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

	bool IsOpen() const noexcept
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

private:
	bool EnsureOpen()
	{
		if (m_desc == InvalidDesc)
		{
			throw std::logic_error("File descriptor is not open");
		}
	}

	int m_desc = InvalidDesc;
};

void HandleAddCommand()
{
}

void ParseAddCommand()
{
	std::string paramsLine;
	std::getline(std::cin, paramsLine);
	int param;
	std::vector<int> params;
	while (std::cin >> param)
	{
		params.emplace_back(param);
	}

	int myPipe[2];
	if (pipe(myPipe) != 0)
	{
		throw std::system_error(errno, std::generic_category());
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		HandleAddCommand();
	}

}

int main()
{
	std::string command;
	while (true)
	{
		std::cin >> command;
		if (command == "add")
		{

		}
		else if (command == "longest_word")
		{

		}
		if (command == "exit")
		{
			break;
		}
	}
}

