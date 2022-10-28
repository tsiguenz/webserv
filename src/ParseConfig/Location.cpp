#include "Location.hpp"

// Object managment

Location::Location() {
}

Location::~Location() {
}

// Accessors

std::string	Location::getPath() const {
	return _path;
}

std::list<std::string>	Location::getAllowedMethods() const {
	return _allowedMethods;
}

std::string	Location::getRedir() const {
	return _redir;
}

std::string	Location::getRoot() const {
	return _root;
}

bool	Location::getAutoIndex() const {
	return _autoIndex;
}

std::string	Location::getIndex() const {
	return _index;
}

std::list<std::string>		Location::getAllowedExtCGI() const {
	return _allowedExtCGI;
}

std::string	Location::getUploadPath() const {
	return _uploadPath;
}

void	Location::setRedir(std::string const& redir) {
	_redir = redir;
}
