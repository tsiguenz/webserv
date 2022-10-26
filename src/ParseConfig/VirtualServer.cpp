#include "VirtualServer.hpp"

// Object managment

VirtualServer::VirtualServer(): _serverName(), _locationList() {
	_ip = "127.0.0.1";
	_port = 8080;
	_clientMaxBodySize = -1; // do not limit by default
}

VirtualServer::~VirtualServer() { }

// Accessors

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

void	VirtualServer::setErrorPage(int const& errorCode, std::string const& path) {
	_errorPages.insert(std::make_pair(errorCode, path));
}

void	VirtualServer::setServerName(std::string const& serverName) {
	_serverName = serverName;
}

void	VirtualServer::setIp(std::string const& ip) {
	_ip = ip;
}

void	VirtualServer::setPort(short const& port) {
	_port = port;
}

void	VirtualServer::setClientMaxBodySize(int const& size) {
	_clientMaxBodySize = size;
}

void	VirtualServer::setNewLocation(Location location) {
	_locationList.push_back(location);
}
