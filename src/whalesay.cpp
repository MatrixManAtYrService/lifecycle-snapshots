#include "whalesay.h"
#include <sstream>

using namespace std;

string Whale::Say(std::shared_ptr<IMessageHolder> message)
{
    std::stringstream ss;
    size_t length = message->GetLength();
	ss << " " << string(length,'_') << '\n';
	ss << "< " << message->GetMessage() << " >" << '\n';
	ss << " " << string(length,'_') << '\n';
	ss << "    \\" << '\n';
	ss << "     \\" << '\n';
	ss << "      \\" << '\n';
	ss << "       \\                     ." << '\n';
	ss << "                             ==" << '\n';
	ss << "                            ===" << '\n';
	ss << "       /\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"___/ ===" << '\n';
	ss << "  ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~" << '\n';
	ss << "       \\______ o          __/" << '\n';
	ss << "        \\    \\        __/" << '\n';
	ss << "          \\____\\______/" << '\n';

    return ss.str();
}
