#include "ConfigParser.hpp"

// Object managment

ConfigParser::ConfigParser(): _fileName(), _fileContent(), _WHITESPACES(" \n\r\t\f\v")
{ _virtualServerList.push_back(VirtualServer()); }

ConfigParser::ConfigParser(std::string const& fileName): _WHITESPACES(" \n\r\t\f\v") {
	_checkFileName(fileName);
	_initAllowedDirectives();
//	_fileName = fileName;
//	_readFile(fileName);
//	_parseFileContent();
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

void	ConfigParser::_initAllowedDirectives() {
	_allowedServerDirectives.push_back("server_name");
	_allowedServerDirectives.push_back("listen");
	_allowedServerDirectives.push_back("error_page");
	_allowedServerDirectives.push_back("client_max_body");
	_allowedLocationDirectives.push_back("methods");
	_allowedLocationDirectives.push_back("return");
	_allowedLocationDirectives.push_back("root");
	_allowedLocationDirectives.push_back("autoindex");
	_allowedLocationDirectives.push_back("index");
	_allowedLocationDirectives.push_back("cgi-allowed-ext");
	_allowedLocationDirectives.push_back("upload-path");
}

bool	ConfigParser::_isServerDirective(std::string const& str) {
	std::list<std::string>::const_iterator	it = _allowedServerDirectives.begin();
	std::list<std::string>::const_iterator	end = _allowedServerDirectives.end();

	for (; it != end; it++)
		if (*it == str)
			return true;
	return false;
}

bool	ConfigParser::_isLocationDirective(std::string const& str) {
	std::list<std::string>::const_iterator	it = _allowedLocationDirectives.begin();
	std::list<std::string>::const_iterator	end = _allowedLocationDirectives.end();

	for (; it != end; it++)
		if (*it == str)
			return true;
	return false;
}

void	ConfigParser::_readFile(std::string const& fileName) {
	std::string	buffer;

	std::ifstream	file(fileName.c_str());
	if (file.good() == false)
		throw std::invalid_argument("Invalid argument: bad file name in _readFile()\n");
	while (file.good() == true && std::getline(file, buffer))
		_fileContent += (buffer += "\n");
	file.close();
	if (_fileContent.empty() == true)
		throw std::invalid_argument("Invalid argument: file is empty in _readFile()\n");
}

// return the next block in { } and update the stringstream
std::string	ConfigParser::_getServerBlock(std::stringstream& sContent) {
	std::string	line;
	std::string	ret;

	while (std::getline(sContent, line)) {
		if (trim(line).empty() == true)
			continue ;
		// TODO: change for accept server[whitespace]{
		if (trim(line) == "server {") {
			ret += line + "\n";
			break ;
		}
		else 
			throw std::invalid_argument("First line of block is bad in _getServerBlock()\n");
	}
	// stop condition for iterate on the function
	if (sContent.good() == false)
		return "";
	bool	inBlock = false;
	while (std::getline(sContent, line)) {
		if (trim(line).empty() == true)
			continue ;
		ret += line + "\n";
		if (line.find("{") != std::string::npos)
			inBlock = true;
		if (trim(line) == "}" && inBlock == false)
			return ret;
		if (trim(line) == "}" && inBlock == true)
			inBlock = false;
	}
	throw std::invalid_argument("Block is not closed in _getServerBlock()\n");
}

void	ConfigParser::_parseFileContent(std::string const& fileContent) {
	std::stringstream	sContent(fileContent);
	std::string			serverBlock;

	while ((serverBlock = _getServerBlock(sContent)) != "") {
		std::cout << "loop" << std::endl;
		_parseServerBlock(serverBlock);
	}
}

VirtualServer	ConfigParser::_parseServerBlock(std::string const& serverBlock) {
	std::string			line;
	std::stringstream	sBlock(serverBlock);
	VirtualServer		vs;

	// skip first line
	std::getline(sBlock, line);
	while (std::getline(sBlock, line)) {
		line = trim(line);
		if (line == "}")
			break ;
		if (line.empty() == true)
			continue ;
		size_t	sep = line.find_first_of(_WHITESPACES);
		if (sep == std::string::npos)
			throw std::invalid_argument("Bad directive syntax _parseServerBlock()\n");
		std::cout << line.substr(0, line.find_first_of(_WHITESPACES)) << std::endl;
	}
	return vs;
}
 
void	ConfigParser::_parseLocationBlock(std::string const& locationBlock) {
	(void) locationBlock;
}
