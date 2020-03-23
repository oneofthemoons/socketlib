#ifndef __SOCKET_LIB_H__
#define __SOCKET_LIB_H__

#include <string>
#include <ios>

#include <sys/types.h>
#include <sys/socket.h>

namespace socketlib
{
    enum AddressFamily
    {
        Inet      = AF_INET,
        InetIpV6  = AF_INET6,
        Unix      = AF_UNIX
    };

    enum Protocol
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM 
    };

    /**
     *  @brief The base socket class.
     */
    class Socket
    {
    public:

        Socket(AddressFamily addressFamily, Protocol protocol);
        virtual void read(char* buffer, std::streamsize sizeBuffer);
        virtual void write(char* data, std::streamsize sizeData);

    protected:
        
        void bindConnection(const std::string& ipAddress, unsigned short port);

        std::string ctorErrnoToString(int _errno) const;
        std::string bindErrnoToString(int _errno) const;

    private:

        class Address
        {
        public:
            AddressFamily  addressFamily;
            std::string    ip;
            unsigned short port;
        }        addressData_;
        int      socketFd_;
        sockaddr address_;

        bool        isIpV4AddressCorrect(const std::string& ipv4) const;
        uint32_t    sIpv4ToHostByteOrder(const std::string& ipv4) const;

        Socket()                   = delete;
        Socket(Socket&)            = delete;
        Socket &operator=(Socket&) = delete;

    };
}

#endif // __SOCKET_LIB_H__