#ifndef __SOCKET_EXCEPTION_H__
#define __SOCKET_EXCEPTION_H__

#include <string>
#include <exception>

namespace socketlib
{
    class SocketException : std::exception
    {

    public:
        
        SocketException(const std::string&);
        const char* what();
        
    private:

        std::string whatMessage;

        SocketException()                            = delete;
        SocketException(const SocketException&)      = delete;
        SocketException &operator=(SocketException&) = delete;
    
    };
}

#define MSG(msg) std::string(__func__) + ": " + msg
#define MSGSTR(msg, str) MSG(msg) + " " + str 

#endif // __SOCKET_EXCEPTION_H__