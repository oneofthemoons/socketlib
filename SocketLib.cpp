#include "SocketLib.h"
#include "SocketException.h"

std::string socketlib::Socket::errnoToString(int _errno) const
{
	switch (_errno)
	{
		case EACCES:
			return "Permission to create a socket of the specified "
				   "type and/or protocol is denied.";
			// missing break
		case EAFNOSUPPORT:
			return "The specified address family is not supported.";
			// missing break
		case EMFILE:
			return "The per-process descriptor table is full.";
			// missing break
		case ENFILE:
			return "The system file table is full.";
			// missing break
		case ENOBUFS:
			return "Insufficient buffer space is available. "
				   "The socket cannot be created until sufficient "
				   "resources are freed.";
			// missing break
		case ENOMEM:
			return "Insufficient memory was available to fulfill the request.";
			// missing break
		case EPROTONOSUPPORT:
			return "The protocol type or the specified protocol is not "
				   "supported within this domain.";
			// missing break
		case EPROTOTYPE:
			return "The socket type is not supported by the protocol.";
			// missing break
		default:
			return "Unexpected error.";
	}
}

socketlib::Socket::Socket(AddressFamily addressFamily, Protocol protocol)
{
	socketFd_ = socket(addressFamily, protocol, 0);
	if (socketFd_ < -1)
		throw SocketException(errnoToString(errno));
}