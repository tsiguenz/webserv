#include "ConfigParser.hpp"

// Object managment

ConfigParser::ConfigParser(): _fileName(), _fileContent()
{ _virtualServerList.push_back(VirtualServer()); }

ConfigParser::ConfigParser(std::string const& fileName) {
	_checkFileName(fileName);
	_fileName = fileName;
//	_parseFile(fileName);
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
	if (fileName.size() <= 5)
		throw std::invalid_argument("Invalid argument: file is too short in _checkFileName()\n");
	if (fileName.substr(fileName.size() - 5, 5) != ".conf")
		throw std::invalid_argument("Invalid argument: file extention is bad in _checkFileName()\n");
}

void	ConfigParser::_parseFile(std::string const& fileName) {
	std::string	line;
	std::stringstream	serverBlock;

	std::ifstream	file(fileName.c_str());
	if (file.good() == false)
		throw std::invalid_argument("Invalid argument: error while opening file in _readFile()\n");
	while (std::getline(file, line)) {
		_fileContent += (line += "\n");
		if (trim(line) == "server {")
			_parseServerBlock(serverBlock);
	}
	file.close();
	if (_fileContent.empty() == true)
		throw std::invalid_argument("Invalid argument: file is empty in _readFile()\n");
}

void	ConfigParser::_parseServerBlock(std::stringstream& serverBlock) {
	std::string	line;

	while (std::getline(serverBlock, line)) {
		_fileContent += (line += "\n");
	}
}
 
//void	ConfigParser::_parseLocationBlock() {
//	
//}
