#include "SocketLib.h"
#include "SocketException.h"

#include <algorithm>
#include <netinet/in.h>

static const size_t IPV4_LEN_MAX = 15;
static const size_t IPV4_LEN_MIN = 7;

std::string socketlib::Socket::ctorErrnoToString(int _errno) const
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
			return "The per-process descriptor table is full. Or ";
				   "the system file table is full.";
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

std::string socketlib::Socket::bindErrnoToString(int _errno) const
{
	switch (_errno)
	{
		case EACCES:
			return "The address is protected, and the user is not "
				   "the superuser.";
			// missing break
		case EADDRINUSE:
			return "The given address is already in use. "
			       "Or (Internet domain sockets) the port number was "
				   "specified as zero in the socket address structure, "
				   "but, upon attempting to bind to an ephemeral port, "
				   "it was determined that all port numbers in the "
				   "ephemeral port range are currently in use. See "
              	   "the discussion of /proc/sys/net/ipv4/ip_local_port_range "
              	   "ip(7).";
			// missing break
		case EBADF:
			return "sockfd is not a valid file descriptor.";
			// missing break
		case EINVAL:
			return "The socket is already bound to an address. Or "
				   "addrlen is wrong, or addr is not a valid address "
				   "for this socket's domain.";
			// missing break
		case EAFNOSUPPORT:
			return "The specified address family is not supported.";
			// missing break
		case ENOTSOCK:
			return "The file descriptor sockfd does not refer to a socket.";
			// missing break
		default:
			return "Unexpected error.";
	}
}

socketlib::Socket::Socket(AddressFamily addressFamily, Protocol protocol)
{
	socketFd_ = socket(addressFamily, protocol, 0);
	if (socketFd_ < -1)
		throw SocketException(MSG(ctorErrnoToString(errno)));
	addressData_.addressFamily = addressFamily;
}

/**
 * @brief Trim string from begin.
 */
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

/**
 * @brief Trim string from end.
 */
static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

/**
 * @brief Trim string from both ends.
 */
static void trim(std::string &s) 
{
    ltrim(s);
    rtrim(s);
}

bool socketlib::Socket::isIpV4AddressCorrect(const std::string &ipv4) const
{
	do
	{
		size_t lenIpv4 = ipv4.size();
		if (lenIpv4 > IPV4_LEN_MAX || lenIpv4 < IPV4_LEN_MIN)
			break;
		
		unsigned short minNDigits = 3,
					   maxNDigits = 0,
					   nDigits = 0,
					   nDots = 0;

		bool isIncorrectSymbolFounded = false;
		for (auto it = ipv4.begin(); it != ipv4.end(); ++it)
		{
			nDigits = 0;
			while (it != ipv4.end() && isdigit(*it))
			{
				++nDigits;
				++it;
			}
			maxNDigits = std::max(maxNDigits, nDigits);
			minNDigits = std::min(minNDigits, nDigits);
			if (it != ipv4.end() && *it == '.')
				++nDots;
			else if (it != ipv4.end())
			{
				isIncorrectSymbolFounded = true;
				break;
			}
		}

		if (maxNDigits > 3 || minNDigits < 1 || isIncorrectSymbolFounded)
			break;
		
		return true;
	} while (false);
	return false;
}

uint32_t socketlib::Socket::sIpv4ToHostByteOrder(const std::string& ipv4) const
{
	uint32_t hostByteOrder = 0;
	uint8_t* pByte = reinterpret_cast<uint8_t*>(&hostByteOrder);
	
	size_t   beg = 0;
	size_t   end = 0;
	for (uint8_t i = 0; i < 4; ++i)
	{
		end = ipv4.find('.');
		*pByte = atoi(ipv4.substr(beg, end - beg).c_str());
		beg = end + 1;
		++pByte;
	}

	return hostByteOrder;
}

void socketlib::Socket::bindConnection(const std::string& ipAddress,
									   unsigned short port)
{
	addressData_.ip   = ipAddress;
	addressData_.port = port;
	trim(addressData_.ip);
	switch (addressData_.addressFamily)
	{
		case Inet:
			if (!isIpV4AddressCorrect(addressData_.ip))
				throw SocketException(MSGSTR("Incorrect IPv4 address", addressData_.ip));

			sockaddr_in* addr     = reinterpret_cast<sockaddr_in*>(&address_);
			addr->sin_family 	  = addressData_.addressFamily;
			addr->sin_addr.s_addr = htonl(sIpv4ToHostByteOrder(addressData_.ip));
			addr->sin_port        = htons(addressData_.port);

			break;
		case InetIpV6:
			// @TODO check IPv6
			break;
		case Unix:
			// @TODO check Unix-address
			break;
	}
	int retBind = bind(socketFd_, &address_, sizeof(address_));
	if (retBind < 0)
		throw SocketException(MSGSTR("Can't bind address to socket", bindErrnoToString(errno)));
}