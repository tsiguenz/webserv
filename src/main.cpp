#ifndef __IS_TEST__

# include "webserv.h"

int	main(int argc, char **argv) {
	if (argc > 2) {
		std::cerr << "Too much arguments. Try with: ./webserv [configuration file]\n";
		return EXIT_FAILURE;
	}
	// parse file
	if (argc == 2)
		std::cerr << "Parse config file: " << argv[1] << std::endl;
	// create server and run it
	try {
		signal(SIGINT, signalHandler); // CTRL / C
		signal(SIGQUIT, SIG_IGN); // CTRL / backslash
		signal(SIGTSTP, SIG_IGN); // CTRL / Z
		Server	serv(8080);

		serv.run();
	} catch (std::exception const& e) {
		std::cerr << e.what();
		std::cerr << "errno = " << errno << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

#endif // __IS_TEST__
