#include "Server.hpp"

Server::Server(int const& port) {
	(void) port;
	_initVirtualServer(8080);
// not working with 2 virtual server
//	_initVirtualServer(4242);
}

Server::~Server() {
	close(_epFd);
	for (std::vector<int>::iterator it = _fdServer.begin(); it < _fdServer.end(); it++)
		close(*it);
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
		if (_isServer(currentEvent.data.fd) == 1)
			_newConnection(currentEvent.data.fd);
		// Client case
		if (_isServer(currentEvent.data.fd) == 0) {
			_parseRequest(currentEvent);
			_sendResponse(currentEvent);
		}
	}
}

int	Server::_isServer(int const& fd) const {
	std::vector<int>::const_iterator	it = _fdServer.begin();
	for (; it < _fdServer.end(); it++)
		if (*it == fd)
			return 1;
	return 0;
}

void	Server::_newConnection(int const& fd) const {
	int	clientSocket = accept(fd, (sockaddr *) NULL, NULL);
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

void	Server::_initVirtualServer(int const& port) {
	sockaddr_in	my_addr;
	int			on = 1;

	bzero(&my_addr, sizeof(sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	int	newFd = socket(AF_INET, SOCK_STREAM, 0);
	_setNonBlocking(newFd);
	if (newFd == -1)
		throw std::runtime_error("error in _initServer::socket()\n");
	if (setsockopt(newFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) == -1)
		throw std::runtime_error("error in _initServer::socketopt()\n");
	if (bind(newFd, (sockaddr *) &my_addr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("error in _initServer::bind()\n");
	if (listen(newFd, BACKLOG) == -1)
		throw std::runtime_error("error in _initServer::listen()\n");
	if ((_epFd = epoll_create(1)) == -1)
		throw std::runtime_error("error in _initServer::epol_create()\n");
	_addEvent(newFd, EPOLLIN);
	_fdServer.push_back(newFd);
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

	//bzero(&ev, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_MOD, fd, &ev) == -1)
		throw std::runtime_error("error in _modEvent::epol_ctl()\n");
}

void	Server::_delEvent(int const& fd) const {
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("error in _delEvent::epol_ctl()\n");
}

// TODO protect fcntl
void	Server::_setNonBlocking(int const& fd) const {
	int oldFlags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, oldFlags | O_NONBLOCK);
}
