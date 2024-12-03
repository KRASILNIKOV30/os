#include "FileDesc.h"
#include "FileWatcher.h"
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <linux/limits.h>
#include <sys/inotify.h>

namespace fs = std::filesystem;

#define EVENT_SIZE ( sizeof(struct inotify_event) )
#define BUF_LEN (EVENT_SIZE + NAME_MAX + 1)

void PrintEvent(inotify_event const& event) {
    if (event.mask & IN_CREATE) {
        std::cout << "File created: " << event.name << std::endl;
    }
    if (event.mask & IN_DELETE) {
        std::cout << "File deleted: " << event.name << std::endl;
    }
    if (event.mask & IN_MODIFY) {
        std::cout << "File modified: " << event.name << std::endl;
    }
    if (event.mask & IN_ATTRIB) {
        std::cout << "File attributes changed: " << event.name << std::endl;
    }
    if (event.mask & IN_MOVED_TO) {
        std::cout << "File moved to: " << event.name << std::endl;
    }
    if (event.mask & IN_MOVED_FROM) {
        std::cout << "File moved from: " << event.name << std::endl;
    }
}

int main(const int argc, char *argv[])
{
    if (argc != 2)
	{
		std::cerr << "Usage: fsmon PATH" << std::endl;
		return 1;
	}

	const fs::path path = argv[1];

	FileDesc inotify(inotify_init());
	FileWatcher watcher(inotify, path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ATTRIB | IN_MOVED_TO | IN_MOVED_FROM);

    std::cout << "Monitoring changes in: " << path << std::endl;

	constexpr auto bufLen = BUF_LEN;
	//wchar
	void* buffer[bufLen];
	size_t space;
	// alignof(inotify_event)
	const auto alignedBuffer = std::align(EVENT_SIZE, bufLen, *buffer, space);
	while (true)
	{
		const auto length = inotify.Read(alignedBuffer, BUF_LEN);
        if (length == -1)
        {
            std::cerr << "read failed: " << strerror(errno) << std::endl;
            break;
        }

        for (size_t i = 0; i < length;)
        {
			const auto* event = static_cast<struct inotify_event*>(alignedBuffer);
            PrintEvent(*event);
            i += EVENT_SIZE + event->len;
        }
    }

    return EXIT_SUCCESS;
}

