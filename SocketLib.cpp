#include "SocketLib.h"
#include "SocketException.h"

#include <algorithm>

static const size_t IPV4_LEN_MAX = 15;
static const size_t IPV4_LEN_MIN = 7;

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
		throw SocketException(MSG(errnoToString(errno)));
	address_.addressFamily = addressFamily;
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

void socketlib::Socket::bindConnection(const std::string& ipAddress,
									   unsigned short port)
{
	address_.ip   = ipAddress;
	address_.port = port;
	trim(address_.ip);
	switch (address_.addressFamily)
	{
		case Inet:
			if (!isIpV4AddressCorrect(address_.ip))
				throw SocketException(MSGSTR("Incorrect IPv4 address", address_.ip));
			break;
		case InetIpV6:
			// @TODO check IPv6
			break;
		case Unix:
			// @TODO check Unix-address
			break;
	}
	
}