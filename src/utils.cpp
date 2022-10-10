# include "webserv.h"

void	ft_exit(std::string str, int ret) {
	std::cerr << str;
	exit(ret);
}

void	ft_exit(std::string str, int ret, std::vector<int> socketFd) {
	std::cerr << str;
	for (std::vector<int>::iterator it = socketFd.begin(); it != socketFd.end(); it++)
		close(*it);
	exit(ret);
}
