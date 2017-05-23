#ifndef parameters_h
#define parameters_h



#include <string>
#include <iostream>

#pragma warning(disable:4505) //https://svn.boost.org/trac/boost/ticket/11920
#include <boost/program_options.hpp>

#define HELP_PARAM "help"
#define MESSAGE_PARAM "message"
#define MESSAGE_DEFAULT "Hello World!"

struct CLI
{
    std::string message;

    CLIparams(int argc, char** argv)
    {
		// define the CLI parameters
		namespace po = boost::program_options;
		po::options_description desc("Usage:");
		desc.add_options()
			(HELP_PARAM, "print this message")
			(HTTP_PORT_PARAM, po::value<std::string>(&message)->default_value(MESSAGE_DEFAULT), "provide a custom message")
			;

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
			// if something went wrong print it
			std::cerr << e.what() << std::endl;
			getchar();
			exit(1);
		}
    };
}


#endif
