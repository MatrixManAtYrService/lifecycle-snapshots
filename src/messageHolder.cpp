#include "messageHolder.h"

MessageHolder::MessageHolder(std::string _message) : message(_message) { }

std::string MessageHolder::GetMessage()
{
    return message;
}

size_t MessageHolder::GetLength()
{
    return message.size();
}
