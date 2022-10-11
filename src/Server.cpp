#include "Server.hpp"

Server::Server(int const& port): _port(port) {
	try { _initServer(); }
	catch (std::exception const& e)
	{ std::cerr << e.what(); }
}

Server::~Server() {
	close(_epFd);
	close(_fdServer);
}

void	Server::run() {
	int	addrlen = sizeof(sockaddr_in);

	while (1) {
		int	nepfd = epoll_wait(_epFd, _events, EVENTS_MAX, 100);
		if (nepfd == -1) {
			std::cerr << "error in epoll_wait()\n";
			break ;
		}
		for (int i = 0; i < nepfd; i++) {
			// server case
			if (_events[i].data.fd == _fdServer) {
				sockaddr_in	clientAddr;
				int	clientSocket = accept(_fdServer, (sockaddr *) &clientAddr, (socklen_t *) &addrlen);
				if (clientSocket == -1) {
					std::cerr << "error in accept()\n";
					continue ;
				}
				_addEvent(clientSocket, EPOLLIN);
			}
			// client case
			else {
				// request case
				if (_events[i].events & EPOLLIN) {
					// TODO: warning if recv don't return all the request
					char		buffer[BUFFER_SIZE];
					bzero(buffer, BUFFER_SIZE);
					recv(_events[i].data.fd, &buffer, BUFFER_SIZE, 0);
					// TODO: parse HTTP request
					std::cout << buffer << std::endl;
					_modEvent(_events[i].data.fd, EPOLLOUT);

				}
				// response case
				if (_events[i].events & EPOLLOUT) {
					// TODO: construct HTTP response
					std::string	str = DUMMY_RESPONSE;
					send(_events[i].data.fd, str.c_str(), str.size(), 0);
					close(_events[i].data.fd);
				}
			}
		}
	}
}

// TODO change std::runtime_error by our own exception
void	Server::_initServer() {
	sockaddr_in	my_addr;
	int	on = 1;

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(_port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	_fdServer = socket(AF_INET, SOCK_STREAM, 0);
	if (_fdServer == -1)
		throw std::runtime_error("error in _initServer::socket()\n");
	if (setsockopt(_fdServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) == -1)
		throw std::runtime_error("error in _initServer::socketopt()\n");
	if (bind(_fdServer, (sockaddr *) &my_addr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("error in _initServer::bind()\n");
	if (listen(_fdServer, BACKLOG) == -1)
		throw std::runtime_error("error in _initServer::listen()\n");
	if ((_epFd = epoll_create(1)) == -1)
		throw std::runtime_error("error in _initServer::epol_create()\n");
	_addEvent(_fdServer, EPOLLIN);
}

// TODO change std::runtime_error by our own exception
void	Server::_addEvent(int& fd, long events) {
	epoll_event	ev;

	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("error in _addEvent::epol_ctl()\n");
}

// TODO change std::runtime_error by our own exception
void	Server::_modEvent(int& fd, long events) {
	epoll_event	ev;

	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_MOD, fd, &ev) == -1)
		throw std::runtime_error("error in _modEvent::epol_ctl()\n");
}

// TODO change std::runtime_error by our own exception
void	Server::_delEvent(int& fd) {
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("error in _delEvent::epol_ctl()\n");
}
