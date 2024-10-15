#pragma once
#include "FileDesc.h"
#include "Request.h"
#include <bit>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>

class PipedChildProcessManager
{
public:
	using PipedAction = std::function<void(FileDesc&&, FileDesc&&)>;

	explicit PipedChildProcessManager(PipedAction&& action)
	{
		int socket[2];
		if (socketpair(PF_LOCAL, SOCK_STREAM, 0, socket) != 0)
		{
			throw std::runtime_error("socketpair() failed");
		}
		m_pid = fork();
		if (m_pid == 0)
		{
			action(FileDesc(socket[1]), FileDesc(socket[1]));
		}
		m_input = FileDesc(socket[0]);
		m_output = FileDesc(socket[0]);
	}

	size_t SendToChild(const void* buffer, const size_t length)
	{
		return m_output.Write(buffer, length);
	}

	size_t ReceiveFromChild(void* buffer, const size_t length)
	{
		return m_input.Read(buffer, length);
	}

	[[nodiscard]] pid_t GetPid() const
	{
		return m_pid;
	}

	~PipedChildProcessManager()
	{
		if (m_pid > 0)
		{
			try
			{
				SendExitRequest();
			}
			catch (...)
			{
				std::cerr << "Failed to kill child process with pid: " << m_pid << std::endl;
			}
		}
	}

private:
	void SendExitRequest()
	{
		auto exitCommand = Request(
		{
			.type = RequestType::EXIT,
		});
		const auto exitCommandBytes = std::bit_cast<uint8_t*>(&exitCommand);
		SendToChild(exitCommandBytes, sizeof(Request));
		if (waitpid(m_pid, nullptr, 0) == -1)
		{
			std::cerr << "waitpid() failed" << std::endl;
		}
	}

	FileDesc m_input;
	FileDesc m_output;
	pid_t m_pid = getpid();
};
