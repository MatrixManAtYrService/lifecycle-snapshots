#ifndef TESTHELPER_H
#define TESTHELPER_H

// warnings in fakeit are note our problem, silence them
#pragma warning(push,0)
#include "FakeIt/single_header/boost/fakeit.hpp"
#pragma warning( pop )	// temporarily

#include <memory>

namespace TestHelper
{
    template<typename T> void blank(T*)
    {
        // does nothing

        //Used to prevent shared_ptr from deleting the Mock (which handles its own clean-up when it goes out of scope)
        //If the mocked object has already been freed, then fakeit tries to do it again and bad stuff happens.
    }

	// call this to create a shared pointer to a mocked object
    template<typename T> static std::shared_ptr<T> PointToMocked(T* mockedObject)
    {
        return std::shared_ptr<T>(mockedObject, blank<T>);
    }
}
#endif
