#ifndef __IS_TEST__

# include "webserv.h"

# define PORT 8080
# define IP "10.12.13.1"
# define BACKLOG 10
# define BUFFER_SIZE 1064 * 128
# define EVENTS_MAX 100
# define DUMMY_RESPONSE "HTTP/1.1 200 OK\nServer: test\nContent-Lenght: 13\nContent-Type: text/html\n\n<H1>webserv</H1>\n"

// add new non blocking fd in epoll
int	addEvent(int& epfd, int& fd, long events) {
	epoll_event	ev;
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "error in addEvent::epoll_ctl()\n";
		return -1;
	}
	return 0;
}

int	modEvent(int& epfd, int& fd, long events) {
	epoll_event	ev;
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
		std::cerr << "error in modEvent::epoll_ctl()\n";
		return -1;
	}
	return 0;
}

int	delEvent(int& epfd, int& fd) {
	if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
		std::cerr << "error in delEvent::epoll_ctl()\n";
		return -1;
	}
	return 0;
}

void	initServer(int& serverSocket) {
	struct sockaddr_in	my_addr;
	int	on = 1;

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	// AF_INET is for IPV4 addresses (use AF_INET6 for IPV6)
	// SOCK_STREAM is for TCP (use SOCK_DGRAM for UDP)
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
		ft_exit("error in socket()\n", 1, serverSocket);
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) == -1)
		ft_exit("error in setsockopt()\n", 1, serverSocket);
	if (bind(serverSocket, (sockaddr *) &my_addr, sizeof(sockaddr)) == -1)
		ft_exit("error in bind()\n", 1, serverSocket);
	if (listen(serverSocket, BACKLOG) == -1)
		ft_exit("error in listen()\n", 1);
}

int	main() {
	int			serverSocket;
	int			addrlen = sizeof(sockaddr_in);
	int			epfd;
	epoll_event	events[EVENTS_MAX];

	initServer(serverSocket);
	if ((epfd = epoll_create(1)) == -1)
		ft_exit("error in epoll_create()\n", 1, serverSocket);
	if (addEvent(epfd, serverSocket, EPOLLIN) == -1) {
		close(epfd);
		ft_exit("", 1, serverSocket);
	}
	while (1) {
		int	nepfd = epoll_wait(epfd, events, EVENTS_MAX, 100);
		if (nepfd == -1) {
			std::cerr << "error in epoll_wait()\n";
			break ;
		}
		for (int i = 0; i < nepfd; i++) {
			// server case
			if (events[i].data.fd == serverSocket) {
				sockaddr_in	clientAddr;
				int	clientSocket = accept(serverSocket, (sockaddr *) &clientAddr, (socklen_t *) &addrlen);
				if (clientSocket == -1) {
					std::cerr << "error in accept()\n";
					continue ;
				}
				if (addEvent(epfd, clientSocket, EPOLLIN) == -1)
					ft_exit("", 1, serverSocket);
						std::cout << "Client connection from: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
			}
			// client case
			else {
				// request case
				if (events[i].events & EPOLLIN) {
					// warning if recv don't return all the request
					char		buffer[BUFFER_SIZE];
					bzero(buffer, BUFFER_SIZE);
					recv(events[i].data.fd, &buffer, BUFFER_SIZE, 0);
					// parse HTTP request
					std::cout << buffer << std::endl;
					modEvent(epfd, events[i].data.fd, EPOLLOUT);

				}
				// response case
				if (events[i].events & EPOLLOUT) {
					// construct HTTP response
					std::string	str = DUMMY_RESPONSE;
					send(events[i].data.fd, str.c_str(), str.size(), 0);
					close(events[i].data.fd);
				}
			}
		}
	}
	close(epfd);
	close(serverSocket);
	return 0;
}

#endif // __IS_TEST__
