#ifndef __IS_TEST__

# include "webserv.h"

# define PORT 8080
# define IP "10.12.13.1"
# define BACKLOG 10
# define HOST 0

void	initServerSocket(std::vector<int>& socketFd) {
	struct sockaddr_in	my_addr;
	int	on = 1;

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	// AF_INET is for IPV4 addresses (use AF_INET6 for IPV6)
	// SOCK_STREAM is for TCP (use SOCK_DGRAM for UDP)
	socketFd.push_back(socket(AF_INET, SOCK_STREAM, 0));
	if (socketFd[0] == -1)
		ft_exit("error in socket()\n", 1, socketFd);
	if (setsockopt(socketFd[HOST], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) == -1)
		ft_exit("error in setsockopt()\n", 1, socketFd);
	if (bind(socketFd[HOST], (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
		ft_exit("error in bind()\n", 1, socketFd);
	if (listen(socketFd[HOST], BACKLOG) == -1)
		ft_exit("error in listen()\n", 1);
}

int	main() {
	std::vector<int>	socketFd;
	struct sockaddr_in	their_addr;
	int		addrlen = sizeof(struct sockaddr_in);
	char	buffer[1024];

	bzero(buffer, 1024);
	initServerSocket(socketFd);
	while (1) {
		// accept is blocking
		socketFd.push_back(accept(socketFd[HOST], (struct sockaddr *) &their_addr, (socklen_t *) &addrlen));
		if (socketFd[1] == -1)
			socketFd.pop_back();
		std::cout << "new connection: " << inet_ntoa(their_addr.sin_addr) << std::endl; // remove
		if (recv(socketFd[1], &buffer, 1024, 0) > 0) {
			std::cout << buffer << std::endl;
			bzero(buffer, 1024);
			send(socketFd[1], "HTTP/1.1 200 OK\nServer: test\nContent-Lenght: 13\nContent-Type: text/html\n\n<H1>webserv</H1>\n", 90, 0);
		}
		close(socketFd[1]);
		socketFd.pop_back();
	}
	return 0;
}

#endif // __IS_TEST__
