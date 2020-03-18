#include <SocketException.h>

SocketLib::SocketException::SocketException(const std::string& whatMessage)
{
    this->whatMessage = whatMessage;
}

const char* SocketLib::SocketException::what()
{
    return whatMessage.c_str();
}