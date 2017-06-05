#ifndef ispeak_h
#define ispeak_h

#include <memory>
#include <string>

class IMessageHolder;

class ISpeak
{
    public:
    virtual std::string Say(std::shared_ptr<IMessageHolder>) = 0;
};

#endif
