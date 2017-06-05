#ifndef IMessageHolder_h
#define IMessageHolder_h

#include <string>

class IMessageHolder
{
public:
    virtual std::string GetMessage() = 0;

    virtual size_t GetLength() = 0;
};

#endif
