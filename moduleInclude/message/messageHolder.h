#ifndef messageHolder_h
#define messageHolder_h

#include <string>
#include "interface/IMessageHolder.h"

// this is a pretty dumb class
// it serves as a dependency that we can inject

class MessageHolder : public IMessageHolder
{
    size_t length;
    std::string message;

public:
    MessageHolder(std::string _message);

    std::string GetMessage();

    size_t GetLength();
};

#endif
