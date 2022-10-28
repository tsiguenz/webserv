#include "VirtualServer.hpp"

// Object managment

VirtualServer::VirtualServer(): _serverNames(), _port(), _locationList() { }

VirtualServer::VirtualServer(VirtualServer const& vs) {
	_serverNames = vs._serverNames;
	_ip = vs._ip;
	_port = vs._port;
	_clientMaxBodySize = vs._clientMaxBodySize;
	_locationList = vs._locationList;
}

VirtualServer::~VirtualServer() { }

// Accessors

std::string	VirtualServer::getErrorPageByCode(int const& errorCode) const {
	return _errorPages.at(errorCode);
}

std::list<std::string>	VirtualServer::getServerNames() const {
	return _serverNames;
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
	_serverNames.push_back(serverName);
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
