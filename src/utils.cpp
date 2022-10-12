# include "webserv.h"

void	signalHandler(int sig) {
	if (sig == SIGINT)
		throw std::runtime_error("Server is closed by user\n");
}
