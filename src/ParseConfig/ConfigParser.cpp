#include "ConfigParser.hpp"

// Object managment

ConfigParser::ConfigParser()
: _fileName(), _fileContent()
{ _virtualServerList.push_back(VirtualServer()); }

ConfigParser::ConfigParser(std::string const& fileName) {
	_checkFileName(fileName);
	_fileName = fileName;
	_readFile();
}

ConfigParser::~ConfigParser() { }

// Accessors

std::list<VirtualServer>	ConfigParser::getVirtualServerList() const
{ return _virtualServerList; }

std::string	ConfigParser::getFileName() const
{ return _fileName; }

std::string	ConfigParser::getFileContent() const
{ return _fileContent; }

// Helper functions

void	ConfigParser::_checkFileName(std::string const& fileName) const {
	(void) fileName;

}

void	ConfigParser::_readFile() {
	std::string	buffer;

	std::ifstream	file(_fileName.c_str());
	if (file.good() == false)
		throw std::invalid_argument("Invalid argument: bad file name in _readFile()\n");
	while (file.good() == true && std::getline(file, buffer))
		_fileContent += (buffer += "\n");
	file.close();
	if (_fileContent.empty() == true)
		throw std::invalid_argument("Invalid argument: file is empty in _readFile()\n");
}
