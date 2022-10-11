#include "Server.hpp"

Server::Server(int const& port): _port(port) {
	try {
		_initVirtualServer();
	} catch (std::exception const& e)
	{ std::cerr << e.what(); }
}

Server::~Server() {
	close(_epFd);
	close(_fdServer);
}

void	Server::run() {
	while (1) {
		int	nbEpollFd = epoll_wait(_epFd, _events, EVENTS_MAX, -1);
		if (nbEpollFd == -1)
			throw std::runtime_error("error in epoll_wait()\n");
		_handleEvent(nbEpollFd);
	}
}

void	Server::_handleEvent(int const& nbEpollFd) const {
	for (int i = 0; i < nbEpollFd; i++) {
		epoll_event	currentEvent = _events[i];
		// Server case
		if (currentEvent.data.fd == _fdServer)
			_newConnection();
		// Client case
		if (currentEvent.data.fd != _fdServer) {
			_parseRequest(currentEvent);
			_sendResponse(currentEvent);
		}
	}
}

void	Server::_newConnection() const {
	int	clientSocket = accept(_fdServer, (sockaddr *) NULL, NULL);
	if (clientSocket == -1)
		throw std::runtime_error("error in accept()\n");
	_addEvent(clientSocket, EPOLLIN);
}

void	Server::_parseRequest(epoll_event const& event) const {
	if (!(event.events & EPOLLIN))
		return ;
	// TODO: warning if recv don't return all the request
	char		buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	recv(event.data.fd, &buffer, BUFFER_SIZE, 0);
	std::cout << buffer << std::endl;
	_modEvent(event.data.fd, EPOLLOUT);
}

void	Server::_sendResponse(epoll_event const& event) const {
	if (!(event.events & EPOLLOUT))
		return ;
	// TODO: construct HTTP response
	std::string	str = DUMMY_RESPONSE;
	send(event.data.fd, str.c_str(), str.size(), 0);
	close(event.data.fd);
}

void	Server::_initVirtualServer() {
	sockaddr_in	my_addr;
	int			on = 1;

	bzero(&my_addr, sizeof(sockaddr_in));
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

void	Server::_addEvent(int const& fd, long const& events) const {
	epoll_event	ev;

	bzero(&ev, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("error in _addEvent::epol_ctl()\n");
}

void	Server::_modEvent(int const& fd, long const& events) const {
	epoll_event	ev;

	bzero(&ev, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_MOD, fd, &ev) == -1)
		throw std::runtime_error("error in _modEvent::epol_ctl()\n");
}

void	Server::_delEvent(int const& fd) const {
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("error in _delEvent::epol_ctl()\n");
}
