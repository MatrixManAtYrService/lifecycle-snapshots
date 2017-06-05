#include "cowsay.h"
#include <sstream>

using namespace std;

string Cow::Say(std::shared_ptr<IMessageHolder> message)
{
    std::stringstream ss;

    size_t length = message->GetLength();
    ss << ' ' << string(length, '_') << '\n';
    ss << '(' << message->GetMessage() <<')' << '\n';
    ss << ' ' << string(length, '-') << '\n';
    ss << "    \\   ^__^"<<'\n';
    ss << "     \\  (oo)\\_______" << '\n';
    ss << "        (__)\\       )\\/\\" << '\n';
    ss << "            ||----w |" << '\n';
    ss << "            ||     ||" << '\n';

    return ss.str();
}
