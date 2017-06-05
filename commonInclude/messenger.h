#ifndef messenger_h
#define messenger_h

#include <memory>
#include "interface/IMessageHolder.h"

class DefaultMessenger : public ISpeak
{
    std::string Say(std::shared_ptr<IMessageHolder> msg)
    {
        return msg->GetMessage();
    }
};

#endif
