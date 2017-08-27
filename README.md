# hello-cpp-linwin

hello-cpp-linwin is a cross platform, unit tested, dependency-injected, command-line-argument parsing, versioned, installable, hello world implementation.  Seriously.

## Why?

I like to start with hello-world and grow my projects from there.  It keeps things bite-sized.  I also like to:

 - Focus on writing testable code from day 1
 - Keep the bulk of my functionailty in libraries for better code reuse
 - Separate implementation headers from interface headers
   - Prevent libraries from including implementation headers of other libraries
   - (This keeps my libraries properly decoupled)
 - Allow developers on multiple platforms to contribute
 - Support command line arguments
 - deploy as a .deb package
 - Indicate the version information from within the software

 
If I truly started from this:

    #include <iostream>
    int(int argc, char** argv)
    {
        std::cout << "Hello World!" << std::endl;
        return 0;
    }

I would have to implement the above bullet points one-by-one.  Instead I prefer to start with the project template in this repo.  That way I can get right into the task at hand, and all of the above points are covered (or at least planned-for).

## How?

#### Cross Platform

[CMakeLists.txt](CMakeLists.txt) is a script that can be run on a variety of platforms (provided cmake is installed).
CMake generates your build environment (see [configure.sh](configure.sh) for details) and keeps it separate from the code (this is called an [out-of-source build](https://cmake.org/Wiki/CMake_FAQ#What_is_an_.22out-of-source.22_build.3F)).

No platform-specific code gets committed, but you can use your .gitignored environment to make changes to files that *are* part of the repository.  This helps keep code in the repo platform agnostic.

Note that if you want to change your compiler settings you must configure CMake to make that change in the generated environment.
That way, the change is also propagated to people on different platforms, and you don't have to maintain them in separate places.
This can be a hassle, but I believe that it's a price worth paying.

#### Unit Tested

[Boost-test](http://www.boost.org/doc/libs/1_64_0/libs/test/doc/html/boost_test/intro.html) is a library that makes it easy to create high quality unit tests.  There is a macro in [CMakeLists.txt](CMakeLists.txt) called AddLibrary that creates libraries in a way that makes creating corresponding test libraries easy.

In addition to boost-test, [FakeIt](https://github.com/eranpeer/FakeIt) is included.  This lets you quickly create dummy-objects which appear to the object-under-test to be one of their dependencies, but are actually under your complete control.

See [test/cowsay.cpp](test/cowsay.cpp) for an example of how this works.  Keep in mind that if your interfaces are clean enough, you shouldnt need to go digging around in the internals of a class to figure out how its behavior should be mocked.


#### Dependency Injected

The AddLibrary macro in [CMakeLists.txt](CMakeLists.txt) looks for a subdirectory: `./moduleInclude/<libraryName>/`
If it exists, it is added to that library's includes.
This is where implementation headers go.

If test sources are provided to AddLibrary, a test library called `test_<libraryName>` is created too.

When I'm doing [dependency injection](https://en.wikipedia.org/wiki/Dependency_injection) I keep the meat of my progam in its libraries, and the primary role of executables is instantiating concrete instances for injection.  This is why the AddExecutable macro allows for the addition of arbitrary include directories, and AddLibrary does not.

Each library gets its own "private" directory of implementation headers, available only to the executables that instantiate classes from those libraries.  CMake will happily let you include implementation headers across different boundaries, but my Macro's will not.  This serves as a reminder: If you wanted that functionality in separate libraries, whatever interfaces you have defined should be sufficient for cross-library communication.  If libraries refer to each other's implementations then why have separate libraries at all?

#### Command-Line-Argument-Parsing

[Boost-program-options](http://www.boost.org/doc/libs/1_63_0/doc/html/program_options.html) is used to parse command line parameters.  This lets you do stuff like this:

    $ hello                                                                                                                                        
    Hello World!                                                                                                        
                                                                                                                        
    $ hello --cow --message "This is the most complicated hello-world ever"                              
     ____________________________________________________________                                                       
    (This is the most complicated hello-world ever)                                                      
     ------------------------------------------------------------                                                       
        \   ^__^                                                                                                        
         \  (oo)\_______                                                                                                
            (__)\       )\/\                                                                                            
                ||----w |                                                                                               
                ||     ||
                
    $ hello --version
    1.2.3.5 2fd4e1c67a2d28fced849ee1bb76e7391b93eb12
    
#### Versioned

Also in [CMakeLists.txt](CMakeLists.txt) there is some code that will grab the current git sha1 and insert it into [commonInclude/version.h](commonInclude/version.h) along with the version number, which is currently hard-coded into [CMakeLists.txt](CMakeLists.txt).  If your version numbers come from an automated build system, you can easily supply these to CMake and have them used by `--version` instead.

#### Installable

Towards the end of [CMakeLists.txt](CMakeLists.txt) you will find the code necessary to create the "install" and "package" build targets.


## Getting Started

### Install Dependencies

In addition to git (duh), you'll need to install boost-program-options, boost-test, and cmake.

##### Linux

On my (ubuntu) install, the following command does the trick:
    
    apt install cmake libboost-test1.58-dev libbost-program-options1.58-dev

Or if you want to install the whole boost suite:

    apt install cmake libboost-all-dev

##### Windows

See the [boost documentation](http://www.boost.org/doc/libs/1_60_0/more/getting_started/windows.html).

Also, you'll need [cmake](https://cmake.org/download/).

If you don't have a bash terminal lying around (git is distributed with one), you'll need one of those too.  I like [MSYS2](http://www.msys2.org/).

### Fetch submodules

After cloning this repo, execute: 

`git submodule update --init`

This will download [FakeIt](https://github.com/eranpeer/FakeIt), which goes (along with any other third-party header-only library you include) goes in `./externalIqnclude/` which is a good place for code that you didn't write.

### Configure

In a bash shell, navigate to the root of this repository and execute:

`./configure.sh`

It will tell you how to use it.  Since I want a debug build using GNU Make, I execute `./configure.sh -g`

### Build

##### GNU make

Navigate to your build directory (indicated by the output of `./configure.sh`) and type `make`

##### Visual Studio

`./configure.sh` should have generated `./build/hello.sln` Open it with visual studio and build it.

### Run Tests

##### Visual Studio

Consider installing the [Boost Test Adapter](https://marketplace.visualstudio.com/items?itemName=ETASGmbH.BoostUnitTestAdapter) for visual studio.  I have had mixed results, but when it works it works well.

In the solution explorer, right click on the project `cow_test` and select debugging > command line parameters.
Use these parameters:

    --run_test=Cow* --catch_system_error=yes --log_level=test_suite

This will run all tests with names that start with Cow (you can modify the expression as necesary).  You can read more about these options [here](http://www.boost.org/doc/libs/1_46_1/libs/test/doc/html/utf/user-guide/runtime-config/reference.html).

##### Linux

You have a few options.  To run all of the tests at once:
 1. navigate to your build directory (`./debug` or `./release` depending on how you invoked  `./configure.sh`) and run `make test`
 2. navigate to your build directory and run `ctest`
 3. navigate to `./<build dir>/testBin` and run `./cow_test --run_test=Cow* --catch_system_error=yes --log_level=test_suite`
 
Here's what's happening
 1. `make` examines the `Makefile` and learns that it should use ctest to run all tests
 2. ctest, a utility bundled with CMake, runs all of the tests according to the parameters that were supplied in the CMake macro `AddLibrary`
 3. boost creates executables (in this case test_cow) which allow for a variety of test-running options
 
 ### Install
 
 Windows users are on their own here.  Linux users can navigate to their build directory and execute the cannonical:
 
     make install
 
 The downside of this method is that it has no uninstaller (though you can always just delete /usr/bin/hello).  
 
 If you're on a debian-based distro, consider using:
 
     make package
     dpkg -i hello_1.2.3.4.deb
     
 To uninstall:
 
     dpkg -r hello
 
### Usage
 
 Execute `hello --help` to see the in-program documentation:
 
     Usage:
      --help                        print this message and exit
      --version                     print the version and exit
      --message arg (=Hello World!) provide a custom message
      --cow                         hear from a cow
      --whale                       hear from a whale
      
### The End

    $ hello --whale --message Goodbye                                                                                                              
     _______                                                                                                            
    < Goodbye >                                                                                                         
     _______                                                                                                            
        \                                                                                                               
         \                                                                                                              
          \                                                                                                             
           \                     .                                                                                      
                                 ==                                                                                     
                                ===                                                                                     
           /""""""""""""""""___/ ===                                                                                    
      ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~                                                                            
           \______ o          __/                                                                                       
            \    \        __/                                                                                           
              \____\______/ 
