#include "FileDesc.h"
#include <stdexcept>
#include <sys/socket.h>

class Socket
{
public:
	explicit Socket(const int socket)
		: m_socket(socket)
	{
	}

	size_t Read(void* buffer, const size_t length)
	{
		return m_socket.Read(buffer, length);
	}

	size_t Write(const void* buffer, const size_t length)
	{
		return m_socket.Write(buffer, length);
	}

private:
	FileDesc m_socket;
};

// Можно сделать функцией
class SocketPair
{
public:
	SocketPair()
	{
		int socket[2];
		if (socketpair(PF_LOCAL, SOCK_STREAM, 0, socket) != 0)
		{
			throw std::runtime_error("socketpair() failed");
		}
		m_parentSocket = socket[0];
		m_childSocket = socket[1];
	}

	// Решить проблему с многократным доступом
	[[nodiscard]] Socket GetParentSocket() const
	{
		return Socket(m_parentSocket);
	}

	[[nodiscard]] Socket GetChildSocket() const
	{
		return Socket(m_childSocket);
	}

private:
	int m_parentSocket;
	int m_childSocket;
};
