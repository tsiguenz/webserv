# include "webserv.h"

void	ft_exit(std::string str, int ret) {
	std::cerr << str;
	exit(ret);
}

void	ft_exit(std::string str, int ret, int serverSocket) {
	std::cerr << str;
	close(serverSocket);
	exit(ret);
}
