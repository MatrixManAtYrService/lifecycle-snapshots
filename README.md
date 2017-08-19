# hello-cpp-linwin

hello-cpp-linwin is a cross platform, unit tested, dependency-injected, command-line-argument parsing, hello world implementation.

## Why?

I like to start with hello-world and grow my projects from there.  But there are problems with this.  I also like to:

 - Focus on writing testable code from day 1
 - Keep the bulk of my functionailty in libraries for better code reuse
 - Separate implementation headers from interface headers
   - Prevent libraries from including implementation headers of other libraries
   - (This keeps my libraries properly decoupled)
 - Allow developers on multiple platforms to contribute
 - Support command line arguments
 - Indicate the version from within the software
 - Ignore certain files
 
If I truly started from this:

    #include <iostream>
    int(int argc, char** argv)
    {
        std::cout << "Hello World!" << std::endl;
        return 0;
    }

I would have to implement the above bullet points one-by-one.  Instead I prefer to start with this project.

## How?

[CMakeLists.txt](CMakeLists.txt) is a script that can be run on any platform (provided cmake is installed).
CMake generates your build environment (see [configure.sh](configure.sh) for details) and keeps it separate from the code (this is called an [out-of-source build](https://cmake.org/Wiki/CMake_FAQ#What_is_an_.22out-of-source.22_build.3F)).
No platform-specific code gets checked in, if you want to do something like link to a library, you must configure CMake to make that change in the generated environment.
That way, the change is also propagated to people on different platforms.
This can be a hassle, but I believe that it's a price worth paying.

Boost-test creates



## Getting Started

### Install Dependencies

In addition to git (duh), you'll need to install boost-program-options, boost-test, and cmake  
If you expect to use other boost libraries, you might as well install them all.

#### Windows

See the [boost documentation](http://www.boost.org/doc/libs/1_60_0/more/getting_started/windows.html).

Also, you'll need [cmake](https://cmake.org/download/).

If you don't have a bash terminal lying around (git is distributed with one), you'll need one of those too.  I like [MSYS2](http://www.msys2.org/).

#### Linux

On my (ubuntu) install, the following command does the trick:
    
    apt install cmake libboost-test1.58-dev libbost-program-options1.58-dev

Or if you want to install the whole boost suite:

    apt install cmake libboost-all-dev

### Fetch submodules

After cloning this repo, execute: 

`git submodule update --init`

This will download [FakeIt](https://github.com/eranpeer/FakeIt) an excellent mocking framework which will make your unit tests easier to write.
FakeIt (and any other header-only third party library you use) goes in ./externalInclude/ so that you 
