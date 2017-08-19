cmake_minimum_required(VERSION 3.4.0)
project(hello C CXX)
message(STATUS "Processing ${CMAKE_CURRENT_LIST_FILE}")

# Gather context
get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
set(TEST_ROOT ${PROJECT_ROOT}/test)
set(TEST_BIN ${CMAKE_BINARY_DIR}/testBin)

# Compiled libraries go in ./lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiled runtimes go in ./build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Use C++ 14
set(CMAKE_CXX_STANDARD 14)

# Use high warning levels and treat warnings as errors
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wconversion -pedantic -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors") # tell gcc to stop after first error
endif()

# If building in release mode, optimize all the way and include no debug symbols
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Global include directories (for shared stuff)
SET(INCLUDE_DIRS "${PROJECT_ROOT}/commonInclude")           # Shared between modules (our code)
SET(INCLUDE_EXT_DIRS "${PROJECT_ROOT}/externalInclude")     # External headers (not our code)

# prefer shared libraries to static linking
add_definitions(-DBOOST_ALL_DYN_LINK)

IF(WIN32)
    # (cmake typically finds boost without help in the linux case)

    # set local boost location
    # example system variable values:
    # BoostRoot : C:\usr\local\Boost
    # BoostVer  : 1_62

    # Find the boost libraries
    SET(BOOST_ROOT "$ENV{BoostRoot}")
    SET(BOOST_LIBRARYDIR "$ENV{BoostRoot}/lib")
    SET(BOOST_INCLUDEDIR "$ENV{BoostRoot}/include/boost-$ENV{BoostVer}")

    # sprinkle dll's where visual studio wants them
    # Note: if you run the program outside of visual studio, be sure to do it from ./build so that Windows finds the dlls 
    file(GLOB boostDlls "$ENV{BoostRoot}/lib/*.dll")
    foreach(boostDll ${boostDlls})
       file(TO_CMAKE_PATH "${boostDll}" correctedBoostDll)
           get_filename_component(boostFileName ${correctedBoostDll} NAME)
           if(boostFileName MATCHES ".*boost_(program_options|unit_test).*\\.dll$")
               configure_file( "${correctedBoostDll}" "${PROJECT_ROOT}/build/${boostFileName}" COPYONLY)
          endif()
    endforeach()

ENDIF()

# Add custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_ROOT}/cmake")

# get git commit sha1
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# TODO: populate these values with real version numbers
SET(MAJOR "1")
SET(MINOR "2")
SET(PATCH "3")
SET(REVISION "5")
SET(COMMIT ${GIT_SHA1})
MESSAGE("Version: ${MAJOR}.${MINOR}.${PATCH}.${REVISION} ${COMMIT}")
configure_file("${PROJECT_ROOT}/commonInclude/version.h.in" "${PROJECT_ROOT}/commonInclude/version.h")
SET(VERSION_STR "${MAJOR}.${MINOR}.${PATCH}.${REVISION}") 



# Gather shared headers
# (must be added explicitly to appear nicely in the VS Solution Explorer)
file(GLOB COMMON_HEADERS "${PROJECT_ROOT}/commonInclude/*.h")
source_group("Header Files\\Common Includes" FILES ${COMMON_HEADERS})

# Use boost unit_test_framework
enable_testing()
find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)
list(APPEND INCLUDE_EXT_DIRS ${Boost_INCLUDE_DIRS})

link_directories(${BOOST_LIBRARYDIR})
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BOOST_LINK_FLAG}" )

# Add include directories
include_directories(${INCLUDE_DIRS})

# Add system include directories (warnings from these headers will be ignored)
include_directories(SYSTEM ${INCLUDE_EXT_DIRS})
# Visual Studio doesn't respect the above distinction,
# use '#pragma warning(disable:<num>)' just before the offending header to work around this

# Add Compile Targets
# ===================

macro(AddExecutable _exeName _sourcesList _includeDirs _linkList)

    # Visual Studio wants headers added explicitly (so they stand out from system headers)
    # Gather them into _moduleHeaders
    set(moduleHeaders "")
    set(temp "")
    foreach(_includeDir ${_includeDirs})
        file(GLOB_RECURSE temp "${PROJECT_ROOT}/${_includeDir}/*.h")
        list(APPEND _moduleHeaders ${temp})
    endforeach()

    # Walk the to-link list
    set(_linkLibs "")
    set(_linkIncludes "")
    set(_libDepends "")
    foreach(_linkParam ${_linkList})
        # If param is a build target
        if(TARGET ${_linkParam})

            # find it, and link to the library
            list(APPEND _linkLibs $<TARGET_LINKER_FILE:${_linkParam}>)

            # grab its includes (so the test library can also include them)
            list(APPEND _linkIncludes ${${_linkParam}_INCL})

            # indicate that this library depends on that one (and should be built first)
            list(APPEND _linkDepends ${_linkParam})

        # Otherwise pass it directly to the linker
        else()
            list(APPEND _linkLibs ${_linkParam})
        endif()
    endforeach()

    add_executable(${_exeName}
        "${_sourcesList};${_moduleHeaders};${COMMON_HEADERS}"
        "")

    foreach(_includeDir ${_includeDirs})
        target_include_directories(${_exeName} PUBLIC ${_includeDir})
    endforeach()

    set_target_properties(${_exeName} PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(${_exeName} ${_linkLibs})

    # Walk the to-link list
    foreach(_linkDepend ${_linkDepends})
        add_dependencies(${_exeName} ${_linkDepend})
    endforeach()

endmacro(AddExecutable)

macro(AddLibrary _libName _sourcesList _testSourcesList _linkList)

    SET(_moduleInclude "${PROJECT_ROOT}/moduleInclude/${_libName}")
    set(_moduleHeaders "")
    file(GLOB_RECURSE _moduleHeaders "${_moduleInclude}/*.h")
    source_group("Header Files\\${_libName} Includes" FILES ${_moduleHeaders})

    # Walk the to-link list
    set(_linkLibs "")
    set(_linkIncludes "")
    set(_libDepends "")
    foreach(_linkParam ${_linkList})
        # If param is a build target
        if(TARGET ${_linkParam})

            # find it, and link to the library
            list(APPEND _linkLibs $<TARGET_LINKER_FILE:${_linkParam}>)

            # grab its includes (so the test library can also include them)
            list(APPEND _linkIncludes ${${_linkParam}_INCL})

            # indicate that this library depends on that one (and should be built first)
            list(APPEND _linkDepends ${_linkParam})

        # Otherwise pass it directly to the linker
        else()
            list(APPEND _linkLibs ${_linkParam})
        endif()
    endforeach()

    # create the build target
    set(libName "${_libName}")
    add_library(${libName}
        "${_sourcesList};${_moduleHeaders};${COMMON_HEADERS};${SHARED_UTILITY_HEADERS};${SHARED_INTERFACE_HEADERS};${SHARED_DATATRANSPORT_HEADERS}"
        "")

    target_include_directories(${libName} PUBLIC ${_moduleInclude})
    set_target_properties(${libName} PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(${libName} ${_linkLibs})


    # if test sources were defined
    if (NOT "${_testSourcesList}" STREQUAL "")
        # create a test executable
        add_executable("${libName}_test" "${_testSourcesList};${TEST_HEADERS};${COMMON_HEADERS};${_moduleHeaders}")
        target_include_directories("${libName}_test" PUBLIC "${_moduleInclude};${_linkIncludes}")
        set_target_properties("${libName}_test" PROPERTIES LINKER_LANGUAGE CXX)

        # link to everything that the library-under-test links to
        list(APPEND _test_link_libraries ${libName})
        list(APPEND _test_link_libraries ${_linkLibs})
        list(APPEND _test_link_libraries ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
        target_link_libraries("${libName}_test" ${_test_link_libraries})
        set_target_properties("${libName}_test" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN})

        # configure ctest to call the test executable the way boost likes it
        # thanks to Eric Scott Barr for this (https://eb2.co/blog/2015/06/driving-boost-dot-test-with-cmake/)
        foreach(_sourceFile ${_testSourcesList})
        file(READ "${_sourceFile}" SOURCE_FILE_CONTENTS)
            string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)"
               FOUND_TESTS ${SOURCE_FILE_CONTENTS})

            foreach(HIT ${FOUND_TESTS})
            string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${HIT})

            add_test(NAME "${_libName}_${TEST_NAME}"
            COMMAND "testBin/${_libName}_test" "--run_test=${TEST_NAME}" "--catch_system_error=yes" "--log_level=test_suite")
            endforeach()
        endforeach()
    endif()

    # Make note of this target's module includes so that test libraries can link to them
    set(${libName}_INCL ${_moduleInclude})

    # Walk the to-link list and make sure this library depends on each item
    foreach(_linkDepend ${_linkDepends})
        add_dependencies(${libName} ${_linkDepend})
    endforeach()

endmacro(AddLibrary)


# AddLibrary("LibName"
#     "sources;go;here"
#     "test;sources;go;here"
#     "Link;To;These")
#
# AddExecutable("ExeName"
#     "sources;go;here"
#     "include;directories"
#     "Link;To;These")

# Uncomment below to dump variables
# (useful for finding variables that contain link targets)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

AddLibrary("message"
    "src/messageHolder.cpp"
    ""
    "")

AddLibrary("cow"
    "src/cowsay.cpp"
    "test/cowsay.cpp"   # a test executable will be generated
    "")

AddLibrary("whale"
    "src/whalesay.cpp"
    ""
    "")

AddExecutable("hello"
    "src/hello.cpp"
    "moduleInclude/cow;moduleInclude/whale;moduleInclude/message"
    "cow;whale;message;${Boost_PROGRAM_OPTIONS_LIBRARY}")

install (TARGETS cow whale message DESTINATION "/usr/lib")
install (TARGETS hello DESTINATION "/usr/bin")

IF(NOT WIN32)
	# determine architecture
	execute_process(COMMAND
	  dpkg-architecture
		-qDEB_HOST_ARCH
	  OUTPUT_VARIABLE
		CMAKE_DEB_HOST_ARCH
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	SET(CPACK_GENERATOR "DEB")

	set(CPACK_DEBIAN_PACKAGE_NAME "hello")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "MatrixManAtYrService (github)")
	set(CPACK_PACKAGE_FILE_NAME "hello_${VERSION_STR}")
	set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "says 'Hello World'")
	set(CPACK_DEBIAN_PACKAGE_VERSION ${VERSION_STR})
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_DEB_HOST_ARCH})
    #set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-chrono1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-log1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-date-time1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-filesystem1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-program-options1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-regex1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-system1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-thread1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-test1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-coroutine1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-context1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libsqlite3-0 (>=3.11.0-1ubuntu1), libssl1.0.0 (>=1.0.2g-1ubuntu4.6), sqlite3 (>=3.11.0-1ubuntu1)")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.58.0 (>=1.58.0+dfsg-5ubuntu3.1), libboost-test1.58.0 (>=1.58.0+dfsg-5ubuntu3.1)")
	#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "pre/post install config stuff here")

ENDIF()

INCLUDE(CPack)
