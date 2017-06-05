#include <iostream>

#include "interface/IMessageHolder.h"
#include "interface/ISpeak.h"

#include "parameters.h"
#include "messenger.h"
#include "cowsay.h"
#include "whalesay.h"
#include "messageHolder.h"

using namespace std;

int main(int argc, char** argv)
{
    // Parse cli parameters
    CLI params(argc, argv);

    // Populate messenger dependency
    std::shared_ptr<ISpeak> messenger;
    switch(params.speaker)
    {
    case Animal::Cow:
        messenger = make_shared<Cow>();
        break;
    case Animal::Whale:
        messenger = make_shared<Whale>();
        break;
    default:
        messenger = make_shared<DefaultMessenger>();
    }

	// Populate message dependency
    shared_ptr<IMessageHolder> message = make_shared<MessageHolder>(params.message);

    // Do Stuff
    cout << messenger->Say(message) << endl;
}
