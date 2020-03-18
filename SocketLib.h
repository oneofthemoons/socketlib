#include <sys/types.h>
#include <sys/socket.h>
#include <string>

namespace SocketLib
{
    class Socket
    {
        Socket();
        virtual void connect(const std::string& ipAddr, uint16_t port);
    };
}