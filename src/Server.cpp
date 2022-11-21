#include "Server.hpp"
#include <signal.h>

Server::Server(ConfigParser const& cp): _requests(), _fdsServer(), _epFd(), _events() {
	_virtualServerList = cp.getVirtualServerList();
	_initEpoll();
	std::list<VirtualServer>::const_iterator	it = _virtualServerList.begin();
	std::list<VirtualServer>::const_iterator	end = _virtualServerList.end();
	for (; it != end; it++)
		_initVirtualServer(*it);
}

Server::~Server() {
	std::vector<int>::const_iterator	it = _fdsServer.begin();
	std::vector<int>::const_iterator	end = _fdsServer.end();
	for (; it != end; it++)
		close(*it);
	close(_epFd);
	if (_requests.empty() == false) {
		std::cerr << "nb request not finished: " << _requests.size() << std::endl;
		std::map<int, Request>::iterator	it = _requests.begin();
		for (; it != _requests.end(); it++)
			close(it->first);
	}
}

void	Server::run() {
	while (1) {
		int	nbEpollFd = epoll_wait(_epFd, _events, EVENTS_MAX, -1);
		if (nbEpollFd == -1)
			throw std::runtime_error("error in epoll_wait()");
		_handleEvent(nbEpollFd);
	}
}

std::vector<int>	Server::getFdsServer() const {
	return _fdsServer;
}

void	Server::_initEpoll() {
	if ((_epFd = epoll_create(1)) == -1)
		throw std::runtime_error("error in epoll_create()");
}


void	Server::_setNonBlocking(int const& fd) const {
	int oldFlags = fcntl(fd, F_GETFL, 0);
	if (oldFlags == -1 || fcntl(fd, F_SETFL, oldFlags | O_NONBLOCK) == -1)
		throw std::runtime_error("error in _setNonBlocking::fcntl()");
}

void	Server::_handleEvent(int const& nbEpollFd) {
	static Request		currentRequest;
	Response	currentResponse;
	for (int i = 0; i < nbEpollFd; i++) {
		epoll_event	currentEvent = _events[i];
		if (_isServer(currentEvent.data.fd) == true)
			_newConnection(currentEvent.data.fd);
		else if (_isServer(currentEvent.data.fd) == false) {
			if (currentEvent.events & EPOLLIN) {
				_parseRequest(currentEvent);
				if (currentRequest.isRequestComplete == true)
					_modEvent(currentEvent.data.fd, EPOLLOUT);
			}
			else if (currentEvent.events & EPOLLOUT) {
				_sendResponse(currentEvent);
				currentRequest = Request();
			}
		}
	}
}

bool	Server::_isServer(int const& fd) const {
	std::vector<int>::const_iterator	it = _fdsServer.begin();
	std::vector<int>::const_iterator	end = _fdsServer.end();
	for (; it != end; it++)
		if (*it == fd)
			return true;
	return false;
}

void	Server::_newConnection(int const& fd) {
	int	clientSocket = accept(fd, (sockaddr *) NULL, NULL);
	if (clientSocket == -1)
		throw std::runtime_error("error in accept()");
	_requests[clientSocket] = Request();
	_addEvent(clientSocket, EPOLLIN | EPOLLOUT);
}

void	Server::_parseRequest(epoll_event const& event) {
	ssize_t	ret = 0;
	int	fd = event.data.fd;
	std::vector<unsigned char>	buffer(BUFFER_SIZE);
	ret = recv(fd, &buffer[0], BUFFER_SIZE, 0);
	if (ret == 0 || ret == -1) {
		_requests.erase(fd);
		close(fd);
		return ;
	}
	// TODO check body size
	_requests[fd].addingBuffer(buffer, ret);
}

void	Server::_sendResponse(epoll_event const& event) {
	std::map<int, Request>::iterator	itReq = _requests.find(event.data.fd);
	if (itReq == _requests.end() || itReq->second.isRequestComplete == false)
		return ;
	itReq->second.printRequest();
	Response	currentResponse(itReq->second, _getVirtualServerByHost((*itReq).second));
	std::cout << currentResponse.response.c_str() << std::endl;
	send(event.data.fd, currentResponse.response.c_str(), currentResponse.response.size(), 0);
	_requests.erase(itReq);
	close(event.data.fd);
}

void	Server::_initVirtualServer(VirtualServer const& vs) {
	sockaddr_in	my_addr;
	int			on = 1;

	bzero(&my_addr, sizeof(sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(vs.getPort());
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	int	newFd = socket(AF_INET, SOCK_STREAM, 0);
	if (newFd == -1)
		throw std::runtime_error("error in _initServer::socket()");
	_setNonBlocking(newFd);
	if (setsockopt(newFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) == -1)
		throw std::runtime_error("error in _initServer::socketopt()");
	if (bind(newFd, (sockaddr *) &my_addr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("error in _initServer::bind()");
	if (listen(newFd, BACKLOG) == -1)
		throw std::runtime_error("error in _initServer::listen()");
	_addEvent(newFd, EPOLLIN);
	_fdsServer.push_back(newFd);
}

void	Server::_addEvent(int const& fd, long const& events) const {
	epoll_event	ev;

	bzero(&ev, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw std::runtime_error("error in _addEvent::epol_ctl()");
}

void	Server::_modEvent(int const& fd, long const& events) const {
	epoll_event	ev;

	bzero(&ev, sizeof(epoll_event));
	ev.data.fd = fd;
	ev.events = events;
	if (epoll_ctl(_epFd, EPOLL_CTL_MOD, fd, &ev) == -1)
		throw std::runtime_error("error in _modEvent::epol_ctl()");
}

void	Server::_delEvent(int const& fd) const {
	if (epoll_ctl(_epFd, EPOLL_CTL_DEL, fd, NULL) == -1)
		throw std::runtime_error("error in _delEvent::epol_ctl()");
}

VirtualServer const 	Server::_selectServer(short const& port, std::string const& ip, std::string const& serverName) const {
	std::list<VirtualServer>	candidatVirtualServer = _virtualServerList;
	std::list<VirtualServer>::iterator it = candidatVirtualServer.begin();
	std::list<VirtualServer>::iterator end = candidatVirtualServer.end();
	std::list<VirtualServer>::iterator itcpy;

	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	while (it != end)
	{
		if (port != (*it).getPort()) {
			itcpy = it;
			it++;
			candidatVirtualServer.erase(itcpy);
		}
		else
			it++;
	}
	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	it = candidatVirtualServer.begin();
	while (it != end)
	{
		if (ip != (*it).getIp()) {
			itcpy = it;
			it++;
			candidatVirtualServer.erase(itcpy);
		}
		else
			it++;
	}
	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	it = candidatVirtualServer.begin();
	if (serverName.empty())
		return(candidatVirtualServer.front());
	std::list<std::string>::iterator itList;
	for (; it != end; it++) {
		bool	isNamePresent = false;
		itList = (*it).getServerNames().begin();
		for (; itList != (*it).getServerNames().end(); itList++)
		{
			if (serverName == (*itList))
				isNamePresent = true;
		}
		if (isNamePresent == false) {
			if (candidatVirtualServer.size() == 1)
				break ;
			candidatVirtualServer.erase(it);
		}
	}
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	return(candidatVirtualServer.front());
}


VirtualServer const	Server::_getVirtualServerByHost(Request const& currentRequest) const {

	std::string	ip;
	std::string	portString;
	std::string	serverName;
	int			port = -1;

	std::map<std::string, std::string>::const_iterator itFields = currentRequest.fieldLines.find("Host");
	if (itFields == currentRequest.fieldLines.end())
		return(_virtualServerList.front());
	std::string	hostName = (*itFields).second;
	size_t	posLocalHost = hostName.find("localhost");
	if (posLocalHost != std::string::npos)
		hostName.replace(posLocalHost, 9, "127.0.0.1");
	size_t	posSeparator = hostName.find_last_of(':');
	if (posSeparator == std::string::npos) {
		if (is_digits(hostName) && hostName.size() < 6)
			port = atoi(hostName.c_str());
		else if (validateIP(hostName))
			ip = hostName;
		else
			serverName = hostName;
	}
	else {
		std::string	firstPart = hostName.substr(0, posSeparator);
		std::string	secondPart = hostName.substr(posSeparator + 1);
		if(validateIP(firstPart))
			ip = firstPart;
		else
			serverName =firstPart;
		if (is_digits(secondPart) && secondPart.size() < 6)
			port = atoi(secondPart.c_str());
		else
			return(_virtualServerList.front());
	}
	if (port > 65535)
		return(_virtualServerList.front());
	if (port == -1)
		port = 8080;
	if (ip.empty())
		ip = "0.0.0.0";
	return(_selectServer(port, ip, serverName));
}
