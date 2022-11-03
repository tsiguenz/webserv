#include "VirtualServer.hpp"

// Object managment

VirtualServer::VirtualServer()
	: _serverNames(), _ip(), _port(), _clientMaxBodySize(), _locationList(),
	_errorPages(), _autoIndex(), _index(), _root(),
	_allowedMethods(), _allowedExtCgi(), _uploadPath(),
	_returnCode(), _returnPath()
{ }

VirtualServer::VirtualServer(VirtualServer const& vs)
	: _serverNames(vs._serverNames), _ip(vs._ip), _port(vs._port),
	_clientMaxBodySize(vs._clientMaxBodySize), _locationList(vs._locationList),
	_errorPages(vs._errorPages), _autoIndex(vs._autoIndex), _index(vs._index),
	_root(vs._root), _allowedMethods(vs._allowedMethods), _allowedExtCgi(vs._allowedExtCgi),
	_uploadPath(vs._uploadPath), _returnCode(vs._returnCode), _returnPath(vs._returnPath)
{ }

VirtualServer::~VirtualServer() { }

// Accessors

std::string	VirtualServer::getErrorPageByCode(int const& errorCode) const {
	return _errorPages.at(errorCode);
}

std::map<int, std::string>	VirtualServer::getErrorPages() const {
	return _errorPages;
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

bool	VirtualServer::getAutoIndex() const {
	return _autoIndex;
}

std::string	VirtualServer::getRoot() const {
	return _root;
}

std::string	VirtualServer::getIndex() const {
	return _index;
}

std::list<std::string>	VirtualServer::getAllowedMethods() const {
	return _allowedMethods;
}

bool	VirtualServer::isAllowedMethod(std::string const& method) const {
	std::list<std::string>::const_iterator it = _allowedMethods.begin();
	std::list<std::string>::const_iterator end = _allowedMethods.end();
	for (; it != end; it++)
		if (*it == method)
			return true;
	return false;
}

std::list<std::string>	VirtualServer::getAllowedExtCgi() const {
	return _allowedExtCgi;
}

bool	VirtualServer::isAllowedExtCgi(std::string const& cgi) const {
	std::list<std::string>::const_iterator it = _allowedExtCgi.begin();
	std::list<std::string>::const_iterator end = _allowedExtCgi.end();
	for (; it != end; it++)
		if (*it == cgi)
			return true;
	return false;
}

std::string	VirtualServer::getUploadPath() const {
	return _uploadPath;
}

int	VirtualServer::getReturnCode() const {
	return _returnCode;
}

std::string	VirtualServer::getReturnPath() const {
	return _returnPath;
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

void	VirtualServer::setAutoIndex(bool const& autoIndex) {
	_autoIndex = autoIndex;
}

void	VirtualServer::setRoot(std::string const& root) {
	_root = root;
}

void	VirtualServer::setIndex(std::string const& index) {
	_index = index;
}

void	VirtualServer::setAllowedMethod(std::string const& method) {
	if (isAllowedMethod(method) == false)
		_allowedMethods.push_back(method);
}

void	VirtualServer::setAllowedExtCgi(std::string const& ext) {
	if (isAllowedExtCgi(ext) == false)
		_allowedExtCgi.push_back(ext);
}

void	VirtualServer::setUploadPath(std::string const& path) {
	_uploadPath = path;
}

void	VirtualServer::setReturn(int const& code, std::string const& path) {
	_returnCode = code;
	_returnPath = path;
}
