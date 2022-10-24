#include "VirtualServer.hpp"

// Object managment

VirtualServer::VirtualServer(): _serverName(), _locationList() {
	_ip = "127.0.0.1";
	_port = 8080;
	_clientMaxBodySize = -1; // do not limit by default
}

VirtualServer::~VirtualServer() { }

// Accessors

// return the content to return in the http response ?
// but hot reload can be broken...
std::string	VirtualServer::getErrorPageByCode(int const& errorCode) const {
	return _errorPages.at(errorCode);
}

std::string	VirtualServer::getServerName() const {
	return _serverName;
}

std::string	VirtualServer::getIp() const {
	return _ip;
}

short	VirtualServer::getPort() const {
	return _port;
}

int	VirtualServer::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

std::list<Location>	VirtualServer::getLocationList() const {
	return _locationList;
}
