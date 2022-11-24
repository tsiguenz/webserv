#include "VirtualServer.hpp"

// Object managment

VirtualServer::VirtualServer()
	:  errorHandler(), _serverNames(), _ip("0.0.0.0"), _port(8080), _clientMaxBodySize(1048576), _locationList(),
	_errorPages(), _autoIndex("off"), _index(), _root(),
	_allowedMethods(), _allowedExtCgi(), _uploadPath(),
	_returnCode(), _returnPath()
{ }

VirtualServer::VirtualServer(VirtualServer const& vs)
	: errorHandler(), _serverNames(vs._serverNames), _ip(vs._ip), _port(vs._port),
	_clientMaxBodySize(vs._clientMaxBodySize), _locationList(vs._locationList),
	_errorPages(vs._errorPages), _autoIndex(vs._autoIndex), _index(vs._index),
	_root(vs._root), _allowedMethods(vs._allowedMethods), _allowedExtCgi(vs._allowedExtCgi),
	_uploadPath(vs._uploadPath), _returnCode(vs._returnCode), _returnPath(vs._returnPath)
{ }

VirtualServer::~VirtualServer() { }

// Accessors

std::string	VirtualServer::getErrorPageByCode(int const& errorCode, std::string const& path) const {
	std::list<Location>::const_iterator	itl = _getFineLocationIt(path);
	if (itl != _locationList.end())
		return (*itl).getErrorPageByCode(errorCode);
	std::map<int, std::string>::const_iterator	itm = _errorPages.find(errorCode);
	return (itm == _errorPages.end()) ? "" : (*itm).second;
}

std::map<int, std::string>	VirtualServer::getErrorPages(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _errorPages;
	return (*it).getErrorPages();
}

std::list<std::string>	VirtualServer::getServerNames() const
{ return _serverNames; }

bool	VirtualServer::isServerName(std::string const& serverName) const {
	std::list<std::string>::const_iterator it = _serverNames.begin();
	std::list<std::string>::const_iterator end = _serverNames.end();
	for (; it != end; it++)
		if (*it == serverName)
			return true;
	return false;
}

std::string	VirtualServer::getIp() const
{ return _ip; }

short	VirtualServer::getPort() const
{ return _port; }

int	VirtualServer::getClientMaxBodySize(std::string const& path) const
{ (void) path; return _clientMaxBodySize; }

std::list<Location>	VirtualServer::getLocationList() const
{ return _locationList; }

std::string	VirtualServer::getAutoIndex(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _autoIndex;
	return (*it).getAutoIndex();
}

std::string	VirtualServer::getRoot(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _root;
	return (*it).getRoot();
}

std::string	VirtualServer::getIndex(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _index;
	return (*it).getIndex();
}

std::list<std::string>	VirtualServer::getAllowedMethods(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _allowedMethods;
	return (*it).getAllowedMethods();
}

bool	VirtualServer::isAllowedMethod(std::string const& method, std::string const& path) const {
	std::list<Location>::const_iterator	itl = _getFineLocationIt(path);
	if (itl != _locationList.end())
		return (*itl).isAllowedMethod(method);
	std::list<std::string>::const_iterator it = _allowedMethods.begin();
	std::list<std::string>::const_iterator end = _allowedMethods.end();
	for (; it != end; it++)
		if (*it == method)
			return true;
	return false;
}

std::list<std::string>	VirtualServer::getAllowedExtCgi(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _allowedExtCgi;
	return (*it).getAllowedExtCgi();
}

bool	VirtualServer::isAllowedExtCgi(std::string const& cgi, std::string const& path) const {
	std::list<Location>::const_iterator	itl = _getFineLocationIt(path);
	if (itl != _locationList.end())
		return (*itl).isAllowedExtCgi(cgi);
	std::list<std::string>::const_iterator it = _allowedExtCgi.begin();
	std::list<std::string>::const_iterator end = _allowedExtCgi.end();
	for (; it != end; it++)
		if (*it == cgi)
			return true;
	return false;
}

std::string	VirtualServer::getUploadPath(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _uploadPath;
	return (*it).getUploadPath();
}

int	VirtualServer::getReturnCode(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _returnCode;
	return (*it).getReturnCode();
}

std::string	VirtualServer::getReturnPath(std::string const& path) const {
	std::list<Location>::const_iterator	it = _getFineLocationIt(path);
	if (it == _locationList.end())
		return _returnPath;
	return (*it).getReturnPath();
}

void	VirtualServer::setErrorPage(int const& errorCode, std::string const& path)
{ _errorPages.insert(std::make_pair(errorCode, path)); }

void	VirtualServer::setServerName(std::string const& serverName)
{ _serverNames.push_back(serverName); }

void	VirtualServer::setIp(std::string const& ip)
{ _ip = ip; }

void	VirtualServer::setPort(short const& port)
{ _port = port; }

void	VirtualServer::setClientMaxBodySize(int const& size)
{ _clientMaxBodySize = size; }

void	VirtualServer::setNewLocation(Location location)
{ _locationList.push_back(location); }

void	VirtualServer::setAutoIndex(std::string const& autoIndex)
{ _autoIndex = autoIndex; }

void	VirtualServer::setRoot(std::string const& root)
{ _root = root; }

void	VirtualServer::setIndex(std::string const& index)
{ _index = index; }

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

void	VirtualServer::setDefaultValueToLocation() {
	std::list<Location>::iterator	it = _locationList.begin();
	std::list<Location>::iterator	end = _locationList.end();

	for (; it != end; it++) {
		if (it->getAutoIndex().empty() == true)
			it->setAutoIndex(_autoIndex);
		if (it->getIndex().empty() == true)
			it->setIndex(_index);
		if (it->getRoot().empty() == true)
			it->setRoot(_root);
		if (it->getUploadPath().empty() == true)
			it->setUploadPath(_uploadPath);
		if (it->getReturnCode() == 0 &&  (*it).getReturnPath().empty() == true)
			it->setReturn(_returnCode, _returnPath);
		std::list<std::string>	allowedMethod = (*it).getAllowedMethods();
		if (allowedMethod.empty() == true) {
			std::list<std::string>::const_iterator it2 = _allowedMethods.begin();
			std::list<std::string>::const_iterator end2 = _allowedMethods.end();
			for (; it2 != end2; it2++)
				it->setAllowedMethod(*it2);
		}
		std::list<std::string>	allowedExt = (*it).getAllowedExtCgi();
		if (allowedExt.empty() == true) {
			std::list<std::string>::const_iterator it2 = _allowedExtCgi.begin();
			std::list<std::string>::const_iterator end2 = _allowedExtCgi.end();
			for (; it2 != end2; it2++)
				it->setAllowedExtCgi(*it2);
		}
		std::map<int, std::string>	errorPage = (*it).getErrorPages();
		if (errorPage.empty() == true) {
			std::map<int, std::string>::const_iterator it2 = _errorPages.begin();
			std::map<int, std::string>::const_iterator end2 = _errorPages.end();
			for (; it2 != end2; it2++)
				it->setErrorPage(it2->first, it2->second);
		}
	}
}

std::list<Location>::const_iterator
VirtualServer::_getFineLocationIt(std::string const& path) const {
	std::list<Location>::const_iterator	it = _locationList.begin();
	std::list<Location>::const_iterator	end = _locationList.end();
	std::string	itPath;
	std::string	locationPath;
	std::string	dir = path.substr(0, path.rfind("/"));
	if (path.empty() == true)
		return end;
	for (; it != end; it++) {
		itPath = (*it).getPath();
		if (itPath == dir)
			return it;
		if (dir.find(itPath) == 0 && locationPath.size() < itPath.size()
				&& (dir[itPath.size()] == '/' || dir[itPath.size() -1] == '/'))
			locationPath = itPath;
	}
	for (it = _locationList.begin(); it != end; it++)
		if (locationPath == (*it).getPath())
			return it;
	return end;
}

