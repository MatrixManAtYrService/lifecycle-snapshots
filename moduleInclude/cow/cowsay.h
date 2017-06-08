#ifndef cowsay_h
#define cowsay_h

#include <memory>
#include <string>

#include "interface/IMessageHolder.h"
#include "interface/ISpeak.h"

class Cow : public ISpeak
{
    public:

    std::string Say(std::shared_ptr<IMessageHolder>);
};

#endif
