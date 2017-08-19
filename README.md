# hello-cpp-linwin

hello-cpp-linwin is a cross platform, unit tested, dependency-injected, command-line-argument parsing, hello world implementation.

## Why?

I like to start with hello-world and grow my projects from there.  I also like to:

 - Focus on writing testable code from day 1
 - Keep the bulk of my functionailty in libraries for better code reuse
 - Separate implementation headers from interface headers
   - Prevent libraries from including implementation headers of other libraries
   - (This keeps my libraries properly decoupled)
 - Allow developers on multiple platforms to contribute
 - Support command line arguments
 - build to a .deb package
 - Indicate the version from within the software

 
If I truly started from this:

    #include <iostream>
    int(int argc, char** argv)
    {
        std::cout << "Hello World!" << std::endl;
        return 0;
    }

I would have to implement the above bullet points one-by-one.  Instead I prefer to start with this project, so I can get right into the task at hand, and all of the above points are covered (or at least planned-for).

## How?

[CMakeLists.txt](CMakeLists.txt) is a script that can be run on any platform (provided cmake is installed).
CMake generates your build environment (see [configure.sh](configure.sh) for details) and keeps it separate from the code (this is called an [out-of-source build](https://cmake.org/Wiki/CMake_FAQ#What_is_an_.22out-of-source.22_build.3F)).
No platform-specific code gets committed, if you want to do something like link to a library, you must configure CMake to make that change in the generated environment.
That way, the change is also propagated to people on different platforms, and you don't have to maintain them in separate places.
This can be a hassle, but I believe that it's a price worth paying.

[Boost-test](http://www.boost.org/doc/libs/1_64_0/libs/test/doc/html/boost_test/intro.html) is a library that makes it easy to create high quality unit tests.  There is a macro in [CMakeLists.txt](CMakeLists.txt) called AddLibrary that creates libraries in a way that makes creating corresponding test libraries easy.

The AddLibrary macro in [CMakeLists.txt](CMakeLists.txt) looks for a subdirectory in 


## Getting Started

### Install Dependencies

In addition to git (duh), you'll need to install boost-program-options, boost-test, and cmake  
If you expect to use other boost libraries, you might as well install them all.

#### Linux

On my (ubuntu) install, the following command does the trick:
    
    apt install cmake libboost-test1.58-dev libbost-program-options1.58-dev

Or if you want to install the whole boost suite:

    apt install cmake libboost-all-dev

#### Windows

See the [boost documentation](http://www.boost.org/doc/libs/1_60_0/more/getting_started/windows.html).

Also, you'll need [cmake](https://cmake.org/download/).

If you don't have a bash terminal lying around (git is distributed with one), you'll need one of those too.  I like [MSYS2](http://www.msys2.org/).

### Fetch submodules

After cloning this repo, execute: 

`git submodule update --init`

This will download [FakeIt](https://github.com/eranpeer/FakeIt) an excellent mocking framework which will make your unit tests easier to write.
For an example of its use, see [./test/cowsay.cpp](test/cowsay.cpp).
FakeIt (and any other header-only third party library you use) goes in `./externalInclude/` which is a good place to store code that you didn't write.

### Configure

In a bash shell, navigate to the root of this repository and execute:

`./configure.sh`

It will tell you how to use it.  Since I want a debug build using GNU Make, I execute `./configure.sh -g`

### Build

#### GNU make

Navigate to your build directory (indicated by the output of `./configure.sh`) and type `make`

#### Visual Studio

`./configure.sh` should have generated `./build/hello.sln` Open it with visual studio and build it.

### Run Tests

#### Visual Studio

Consider installing the [Boost Test Adapter](https://marketplace.visualstudio.com/items?itemName=ETASGmbH.BoostUnitTestAdapter] for visual studio.  I have had mixed results, but when it works it works well.

In the solution explorer, right click on the project `cow_test` and select debugging > command line parameters.
Use these parameters:

    --run_test=Cow* --catch_system_error=yes --log_level=test_suite

This will run all tests with names that start with Cow (you can modify the expression as necesary).  You can read more about these options [here](http://www.boost.org/doc/libs/1_46_1/libs/test/doc/html/utf/user-guide/runtime-config/reference.html).

#### Linux

You have a few options.  To run all of the tests at once:
 1. navigate to your build directory (`./debug` or `./release` depending on how you invoked  `./configure.sh`) and run `make test`
 2. navigate to your build directory and run `ctest`
 3. navigate to `./<build dir>/testBin` and run `./cow_test --run_test=Cow* --catch_system_error=yes --log_level=test_suite`
 
Here's what's happening
 1. `make` examines the `Makefile` and learns that it should use ctest to run all tests
 2. ctest, a utility bundled with CMake, runs all of the test
 3. boost creates executables (in this case test_cow)
