#ifndef parameters_h
#define parameters_h

#include <string>
#include <iostream>

#pragma warning(disable:4505) //https://svn.boost.org/trac/boost/ticket/11920
#include <boost/program_options.hpp>
#include "version.h"

#define HELP_PARAM "help"
#define VERSION_PARAM "version"
#define COW_PARAM "cow"
#define WHALE_PARAM "whale"
#define MESSAGE_PARAM "message"
#define MESSAGE_DEFAULT "Hello World!"

// for help with boost::program_options see:
// http://www.boost.org/doc/libs/1_63_0/doc/html/program_options/tutorial.html

enum class Animal {Cow, Whale, Terminal};

struct CLI
{
    std::string message;
    Animal speaker;

    CLI(int argc, char** argv)
    {
        bool isCow = false;
        bool isWhale = false;
        bool printVersion = false;

		// define the CLI parameters
		namespace po = boost::program_options;
		po::options_description desc("Usage:");
		desc.add_options()
			(HELP_PARAM, "print this message and exit")
			(VERSION_PARAM, po::bool_switch(&printVersion), "print the version and exit")
			(MESSAGE_PARAM, po::value<std::string>(&message)->default_value(MESSAGE_DEFAULT), "provide a custom message")
			(COW_PARAM, po::bool_switch(&isCow), "hear from a cow")
			(WHALE_PARAM, po::bool_switch(&isWhale), "hear from a whale")
			;

        // read the CLI parameters
		po::variables_map vm;
		try
		{
			// read them from the parameters
			const po::positional_options_description p;
			po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
			po::notify(vm);

			// if the user asked for help, print it
			if (vm.count(HELP_PARAM)) {
				std::cout << desc << "\n";
				exit(0);
			}
		}
		catch (const boost::program_options::error& e)
		{
			// if something went wrong print it and exit
			std::cerr << e.what() << std::endl;
			getchar();
			exit(1);
		}

        if (printVersion)
        {
            PrintVersion();
            exit(0);
        }


        // CLI post-processing
        if (isCow)
            speaker = Animal::Cow;

        else if (isWhale)
            speaker = Animal::Whale;

        else
            speaker = Animal::Terminal;
    };
};


#endif
