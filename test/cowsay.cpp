#ifndef test_cowsay_h
#define test_cowsay_h

#define BOOST_TEST_MODULE "Sky Core Tests"          // only needed once per test executable
#include "testHelper.h"

#include "cowsay.h"                                 // contains the class under test

#include <boost/test/unit_test.hpp>                 // include this last

using namespace std;
using namespace fakeit;

BOOST_AUTO_TEST_CASE(Cow_message_integrity)
{
    // mock any dependencies that aren't internal to this library
    Mock<IMessageHolder> mockHolder;
    When(Method(mockHolder, GetMessage)).AlwaysReturn("moo");
    When(Method(mockHolder, GetLength)).AlwaysReturn(3);

    auto message = TestHelper::PointToMocked<IMessageHolder>(&mockHolder.get());

    Cow cow;
    auto said = cow.Say(message);

/*
  v---- checking for this

(moo)

    \   ^__^          v----- and this
     \  (oo)\_______
        (__)\       )\/\
            ||----w |
            ||     ||

                  ^--- and this
*/

    BOOST_CHECK_MESSAGE(said.find("moo") != string::npos, "If the message was moo then the cow should have said moo");
    BOOST_CHECK_MESSAGE(said.find("w") != string::npos, "The cow should have had utters");
    BOOST_CHECK_MESSAGE(said.find("\\/\\") != string::npos, "The cow should have had a tail");
}

BOOST_AUTO_TEST_CASE(Cow_empty_message)
{
    // mock any dependencies that aren't internal to this library
    Mock<IMessageHolder> mockHolder;
    When(Method(mockHolder, GetMessage)).AlwaysReturn("");
    When(Method(mockHolder, GetLength)).AlwaysReturn(0);

    auto message = TestHelper::PointToMocked<IMessageHolder>(&mockHolder.get());

    Cow cow;
    auto said = cow.Say(message);

/*
 v---   checking for this
()

    \   ^__^
     \  (oo)\_______
        (__)\       )\/\
            ||----w |
            ||     ||
*/

    BOOST_CHECK_MESSAGE(said.find("()") != string::npos, "If the message was empty, the speech-bubble should have been too");
}

#endif
