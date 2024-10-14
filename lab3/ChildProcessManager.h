#pragma once
#include "FileDesc.h"
#include "Request.h"

#include <bit>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>

class PipedChildProcessManager
{
public:
	using PipedAction = std::function<void(FileDesc&&, FileDesc&&)>;

	explicit PipedChildProcessManager(PipedAction&& action)
	{
		int myPipe1[2];
		int myPipe2[2];
		if (pipe(myPipe1) != 0 || pipe(myPipe2) != 0)
		{
			throw std::runtime_error("pipe() failed");
		}
		m_pid = fork();
		if (m_pid == 0)
		{
			action(FileDesc(myPipe1[0]), FileDesc(myPipe2[1]));
		}
		m_input = FileDesc(myPipe2[0]);
		m_output = FileDesc(myPipe1[1]);
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
