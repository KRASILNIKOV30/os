#pragma once
#include "FileDesc.h"
#include <cstring>
#include <iostream>

class FileWatcher
{
	constexpr static int InvalidDesc = -1;

public:
	FileWatcher(FileDesc const& inotify, fs::path const& path, const uint32_t mask)
		: m_desc(inotify.AddWatcher(path, mask))
		, m_inotify(inotify)
	{
		if (m_desc == -1)
		{
			std::cerr << "inotify_add_watch failed: " << strerror(errno) << std::endl;
		}
	}

	FileWatcher(FileWatcher&& other) noexcept
		: m_desc(std::exchange(other.m_desc, InvalidDesc))
		, m_inotify(other.m_inotify)
	{
	}

	FileWatcher(const FileWatcher& other) = delete;
	FileWatcher& operator=(const FileWatcher& other) = delete;

	~FileWatcher()
	{
		if (m_desc != InvalidDesc)
		{
			m_inotify.RemoveWatcher(m_desc);
		}
		m_desc = InvalidDesc;
	}

private:
	int m_desc = InvalidDesc;
	FileDesc const& m_inotify;
};
