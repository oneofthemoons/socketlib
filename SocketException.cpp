#include <SocketException.h>

socketlib::SocketException::SocketException(const std::string& whatMessage)
{
    this->whatMessage = whatMessage;
}

const char* socketlib::SocketException::what()
{
    return whatMessage.c_str();
}