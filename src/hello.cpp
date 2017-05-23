#include "parameters.h"
#include "cowsay.h"

int main(int argc, char** argv)
{
    CLI params(argc, argv);

    Cow cow;
    cow.opinion = params.message;
    cow.Moo();
}
