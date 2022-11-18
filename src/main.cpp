#ifndef __IS_TEST__

# include "Server.hpp"
# include "ConfigParser.hpp"
# include <cerrno>
# include <csignal>

int	main(int argc, char **argv) {
	if (argc > 2) {
		std::cerr << "Too much arguments. Try with: ./webserv [configuration file]\n";
		return EXIT_FAILURE;
	}
	try {
		ConfigParser	cp;
		if (argc == 2)
			cp.parseFile(argv[1]);
		else
			cp.parseFile("config/default.conf");
		Server	serv(cp);
		signal(SIGINT, signalHandler);
		signal(SIGPIPE, signalHandler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		serv.run();
	} catch (std::exception const& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "errno = " << errno << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

#endif // __IS_TEST__
