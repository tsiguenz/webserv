#include "Location.hpp"

// Object managment

Location::Location()
	: _path(), _errorPages(), _autoIndex(), _index(), _root(),
	_allowedMethods(), _allowedExtCgi(), _uploadPath(),
	_returnCode(), _returnPath()
{ }

Location::Location(Location const& l)
	: _path(l._path), _errorPages(l._errorPages), _autoIndex(l._autoIndex),
	_index(l._index), _root(l._root), _allowedMethods(l._allowedMethods),
	_allowedExtCgi(l._allowedExtCgi), _uploadPath(l._uploadPath),
	_returnCode(l._returnCode), _returnPath(l._returnPath)
{ }

Location::~Location() { }

// Accessors

std::string	Location::getPath() const
{ return _path; }

std::string	Location::getErrorPageByCode(int const& errorCode) const {
	std::map<int, std::string>::const_iterator	itm = _errorPages.find(errorCode);
	return (itm == _errorPages.end()) ? "" : (*itm).second;
}

std::map<int, std::string>	Location::getErrorPages() const
{ return _errorPages; }

std::string	Location::getAutoIndex() const
{ return _autoIndex; }

std::string	Location::getRoot() const
{ return _root; }

std::string	Location::getIndex() const
{ return _index; }

std::list<std::string>	Location::getAllowedMethods() const
{ return _allowedMethods; }

bool	Location::isAllowedMethod(std::string const& method) const {
	std::list<std::string>::const_iterator it = _allowedMethods.begin();
	std::list<std::string>::const_iterator end = _allowedMethods.end();
	for (; it != end; it++)
		if (*it == method)
			return true;
	return false;
}

std::list<std::string>	Location::getAllowedExtCgi() const
{ return _allowedExtCgi; }

bool	Location::isAllowedExtCgi(std::string const& cgi) const {
	std::list<std::string>::const_iterator it = _allowedExtCgi.begin();
	std::list<std::string>::const_iterator end = _allowedExtCgi.end();
	for (; it != end; it++)
		if (*it == cgi)
			return true;
	return false;
}

std::string	Location::getUploadPath() const
{ return _uploadPath; }

int	Location::getReturnCode() const
{ return _returnCode; }

std::string	Location::getReturnPath() const
{ return _returnPath; }

void	Location::setPath(std::string const& path)
{ _path = path; }

void	Location::setErrorPage(int const& errorCode, std::string const& path)
{ _errorPages.insert(std::make_pair(errorCode, path)); }

void	Location::setAutoIndex(std::string const& autoIndex)
{ _autoIndex = autoIndex; }

void	Location::setRoot(std::string const& root)
{ _root = root; }

void	Location::setIndex(std::string const& index)
{ _index = index; }

void	Location::setAllowedMethod(std::string const& method) {
	if (isAllowedMethod(method) == false)
		_allowedMethods.push_back(method);
}

void	Location::setAllowedExtCgi(std::string const& ext) {
	if (isAllowedExtCgi(ext) == false)
		_allowedExtCgi.push_back(ext);
}

void	Location::setUploadPath(std::string const& path)
{ _uploadPath = path; }

void	Location::setReturn(int const& code, std::string const& path) {
	_returnCode = code;
	_returnPath = path;
}
