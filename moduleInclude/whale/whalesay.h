#ifndef whalesay_h
#define whalesay_h

#include <memory>
#include <string>

#include "interface/ISpeak.h"
#include "interface/IMessageHolder.h"

class Whale : public ISpeak
{
    public:

    std::string Say(std::shared_ptr<IMessageHolder>);
};


    #endif
