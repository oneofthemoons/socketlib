#include <exception>
#include <string>

namespace SocketLib
{
    class SocketException : std::exception
    {
        std::string whatMessage;

        SocketException()                            = delete;
        SocketException(const SocketException&)      = delete;
        SocketException &operator=(SocketException&) = delete;


    public:
        SocketException(const std::string&);
        const char* what();
    };
}