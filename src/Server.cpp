#include "Server.hpp"
#include <signal.h>
Server::Server(ConfigParser const& cp) {
	// std::cout << "File config is: " << cp.getFileName() << std::endl;
	_virtualServerList = cp.getVirtualServerList();
	_initEpoll();
	std::list<VirtualServer>::const_iterator	it = _virtualServerList.begin();
	std::list<VirtualServer>::const_iterator	end = _virtualServerList.end();
	// if multiple port defined, init can fail
	for (; it != end; it++)
		_initVirtualServer(*it);
}

Server::~Server() {
	std::vector<int>::const_iterator	it = _fdsServer.begin();
	std::vector<int>::const_iterator	end = _fdsServer.end();
	for (; it != end; it++)
		close(*it);
	close(_epFd);
}

void	Server::run() {
	while (1) {
		signal(SIGPIPE, signalHandler); 
		int	nbEpollFd = epoll_wait(_epFd, _events, EVENTS_MAX, -1);
		if (nbEpollFd == -1)
			throw std::runtime_error("error in epoll_wait()\n");
		_handleEvent(nbEpollFd);
	}
}

std::vector<int>	Server::getFdsServer() const {
	return _fdsServer;
}

void	Server::_initEpoll() {
	if ((_epFd = epoll_create(1)) == -1)
		throw std::runtime_error("error in epoll_create()\n");
}


void	Server::_setNonBlocking(int const& fd) const {
	int oldFlags = fcntl(fd, F_GETFL, 0);
	if (oldFlags == -1 || fcntl(fd, F_SETFL, oldFlags | O_NONBLOCK) == -1)
		throw std::runtime_error("error in _setNonBlocking::fcntl()\n");
}

void	Server::_handleEvent(int const& nbEpollFd) const {
	Request		currentRequest;
	Response	currentResponse;
	for (int i = 0; i < nbEpollFd; i++) {
		epoll_event	currentEvent = _events[i];
		if (currentEvent.events & EPOLLERR)
			throw std::runtime_error("error EPOLLERR\n");
		else if (_isServer(currentEvent.data.fd) == true)
			_newConnection(currentEvent.data.fd);
		else if (_isServer(currentEvent.data.fd) == false) {
			if (currentEvent.events & EPOLLIN) {
				currentRequest = Request(_parseRequest(currentEvent));
				currentResponse	= Response(currentRequest, getVirtualServerByHost(currentRequest));
				_modEvent(currentEvent.data.fd, EPOLLOUT);
				// currentResponse.printResponse();
			}
			if (currentEvent.events & EPOLLOUT)
				_sendResponse(currentEvent, currentResponse);
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

void	Server::_newConnection(int const& fd) const {
	int	clientSocket = accept(fd, (sockaddr *) NULL, NULL);
	if (clientSocket == -1)
		throw std::runtime_error("error in accept()\n");
	_addEvent(clientSocket, EPOLLIN | EPOLLOUT);
}

Request	Server::_parseRequest(epoll_event const& event) const {
	// TODO: warning if recv don't return all the request
	// char		buffer[BUFFER_SIZE];
	std::vector<char> buffer2(BODY_MAX_SIZE + HEADER_MAX_SIZE, '\0');
	// std::cout << "SIZE BUFFER" << buffer2.size() << std::endl;
	// std::cout << "SIZE BUFFER" << buffer2.max_size() << std::endl;

	// bzero(buffer, BUFFER_SIZE);
	size_t end = recv(event.data.fd, &buffer2[0], BODY_MAX_SIZE, 0);
	std::cout << "--------------RAWREQUEST----------------"<< end << std::endl;
	// for (std::vector<char>::iterator it = buffer2.begin(); it != buffer2.begin() + end;it++){
	// 	std::cout << (*it);
	// }
	std::cout << std::endl;
	Request currentRequest(buffer2);
	// if (currentRequest.badRequest == true) {
	// 	std::cout << "400 \n";
	// 	return;
	// }
	// currentRequest.printRequest();
	return (currentRequest);

}

void	Server::_sendResponse(epoll_event const& event, Response const& currentResponse) const {
	send(event.data.fd, currentResponse.response.c_str(),  currentResponse.response.size(), 0);
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
		throw std::runtime_error("error in _initServer::socket()\n");
	_setNonBlocking(newFd);
	if (setsockopt(newFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(int)) == -1)
		throw std::runtime_error("error in _initServer::socketopt()\n");
	if (bind(newFd, (sockaddr *) &my_addr, sizeof(sockaddr)) == -1)
		throw std::runtime_error("error in _initServer::bind()\n");
	if (listen(newFd, BACKLOG) == -1)
		throw std::runtime_error("error in _initServer::listen()\n");
	_addEvent(newFd, EPOLLIN);
	_fdsServer.push_back(newFd);
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

VirtualServer const 	Server::selectServer(short const & port, std::string const & ip, std::string const & serverName) const {

	// (void)port;
	// (void)ip;
	// (void)serverName;

	std::list<VirtualServer>	candidatVirtualServer = _virtualServerList;
	std::list<VirtualServer>::iterator it = candidatVirtualServer.begin();
	std::list<VirtualServer>::iterator end = candidatVirtualServer.end();
	std::list<VirtualServer>::iterator itcpy;

	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	
	// std::cout << "COUCOU" <<std::endl;
	for (; it != end;)
	{
		if (port != (*it).getPort()) {
			itcpy = it;
			it++;
			candidatVirtualServer.erase(itcpy);
		}
		else
			it++;
	}
	// std::cout << "COUCOU1" <<std::endl;

	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	it = candidatVirtualServer.begin();
	for (; it != end;)
	{
		if (ip != (*it).getIp()) {
			itcpy = it;
			it++;
			candidatVirtualServer.erase(itcpy);
		}
		else
			it++;
	}
	// std::cout << "COUCOU2" <<std::endl;

	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	it = candidatVirtualServer.begin();
	if (serverName.empty())
		return(candidatVirtualServer.front());
	for (; it != end; it++)
	{
		bool isNamePresent= false;
		for (std::list<std::string>::const_iterator itVec = (*it).getServerNames().begin(); itVec != (*it).getServerNames().end(); itVec++)
		{
			if (serverName == (*itVec)) {
				isNamePresent = true;
			}			
		}
			if (isNamePresent == false){
				if (candidatVirtualServer.size() == 1)
					break ;
				candidatVirtualServer.erase(it);
			}
	}
	// if (!candidatVirtualServer.empty())
	// 	std::cout << "SERVOR NAME SELECTEDddddd: "<< candidatVirtualServer.front().getServerNames().front() << std::endl;
	// else
	// 	std::cout << "SERVOR NAME SELECTED: "<< _virtualServerList.front().getServerNames().front() << std::endl;
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	return(candidatVirtualServer.front());
	

	// en attendant le debug:
	// return(_virtualServerList.front());
}


VirtualServer const 	Server::getVirtualServerByHost(Request const & currentRequest) const {
	
	std::string	ip;
	std::string	portString;
	std::string	serverName;
	int		port = -1;

	std::map<std::string, std::string>::const_iterator itFields = currentRequest.fieldLines.find("Host");
	if (itFields == currentRequest.fieldLines.end())
		return(_virtualServerList.front());

	std::string	hostName = (*itFields).second;
	size_t posLocalHost = hostName.find("localhost");
	if (posLocalHost != std::string::npos) {
		hostName.replace(posLocalHost, 9, "127.0.0.1");
	}
	
	size_t posSeparator = hostName.find_last_of(':');
	if (posSeparator == std::string::npos) {
		if (is_digits(hostName) && hostName.size() < 6){
			port = atoi(hostName.c_str());
		}	
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
	//securiser overflow port
	if (port > 65535)
		return(_virtualServerList.front());
	//set les val par defaut
	if (port == -1)
		port = 8080;
	if (ip.empty())
		ip = "0.0.0.0";
	// std::cout << BBLUE "\nIP:" << ip << " PORT:" << port << " SERVER NAME:" << serverName <<  WHITE <<std::endl; //debug
	return(selectServer(port, ip, serverName));
}
