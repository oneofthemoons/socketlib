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
        Socket()                   = delete;
        Socket(Socket&)            = delete;
        Socket &operator=(Socket&) = delete;
    public:

        Socket(AddressFamily addressFamily, Protocol protocol);
        virtual void read(char* buffer, std::streamsize sizeBuffer);
        virtual void write(char* data, std::streamsize sizeData);

    protected:
        
        std::string errnoToString(int _errno) const;

    private:

        int socketFd_;
    };
}

#endif // __SOCKET_LIB_H__