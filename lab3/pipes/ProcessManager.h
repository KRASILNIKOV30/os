#pragma once
#include "Child.h"
#include "Request.h"
#include <bit>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>

class ProcessManager
{
public:
	explicit ProcessManager(SocketPair&& socketPair)
		: m_socketPair(socketPair)
		  , m_socket(m_socketPair.GetParentSocket())
	{
		m_pid = fork();
		if (m_pid == 0)
		{
			Child child(m_socketPair.GetChildSocket());
			child.Run();
		}
	}

	size_t SendToChild(const void* buffer, const size_t length)
	{
		return m_socket.Write(buffer, length);
	}

	size_t ReceiveFromChild(void* buffer, const size_t length)
	{
		return m_socket.Read(buffer, length);
	}

	[[nodiscard]] pid_t GetPid() const
	{
		return m_pid;
	}

	~ProcessManager()
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
		auto exitRequest = Request(
		{
			.type = RequestType::EXIT,
		});
		const auto exitCommandBytes = std::bit_cast<uint8_t*>(&exitRequest);
		SendToChild(exitCommandBytes, sizeof(Request));
		if (waitpid(m_pid, nullptr, 0) == -1)
		{
			std::cerr << "waitpid() failed" << std::endl;
		}
	}

	SocketPair m_socketPair;
	Socket m_socket;
	pid_t m_pid = getpid();
};
