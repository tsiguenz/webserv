# include "webserv.h"

void	signalHandler(int sig) {
	if (sig == SIGINT)
		throw std::runtime_error("\nServer is closed by user\n");
}
