#include "ConfigParser.hpp"

// Object managment

# ifndef __IS_TEST__

ConfigParser::ConfigParser(): _fileName(), _fileContent(), _WHITESPACES(" \n\r\t\f\v") {
	_fileName = "config/default.conf";
	_checkFileName(_fileName);
	_initAllowedDirectives();
	_readFile(_fileName);
	_parseFileContent(_fileContent);
}

# else

ConfigParser::ConfigParser(): _fileName(), _fileContent(), _WHITESPACES(" \n\r\t\f\v") {
	_initAllowedDirectives();
}

# endif // __IS_TEST__

ConfigParser::ConfigParser(std::string const& fileName): _WHITESPACES(" \n\r\t\f\v") {
	_checkFileName(fileName);
	_initAllowedDirectives();
	_fileName = fileName;
	_readFile(_fileName);
	_parseFileContent(_fileContent);
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
	// VirtualServer
	_allowedServerDirectives.push_back("server_name");
	_allowedServerDirectives.push_back("listen");
	_allowedServerDirectives.push_back("client_max_body");
	_allowedServerDirectives.push_back("error_page");
	_allowedServerDirectives.push_back("index");
	_allowedServerDirectives.push_back("root");
	_allowedServerDirectives.push_back("autoindex");
	_allowedServerDirectives.push_back("return");
	_allowedServerDirectives.push_back("cgi_allowed_ext");
	_allowedServerDirectives.push_back("upload_path");
	_allowedServerDirectives.push_back("methods");
	// Location
	_allowedLocationDirectives.push_back("error_page");
	_allowedLocationDirectives.push_back("index");
	_allowedLocationDirectives.push_back("root");
	_allowedLocationDirectives.push_back("autoindex");
	_allowedLocationDirectives.push_back("return");
	_allowedLocationDirectives.push_back("cgi_allowed_ext");
	_allowedLocationDirectives.push_back("upload_path");
	_allowedLocationDirectives.push_back("methods");
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

// virtual server overload
void	ConfigParser::_parseDirective(std::string const& line, VirtualServer& vs) {
	std::string	tLine = trim(line);
	std::string	directiveName = tLine.substr(0, tLine.find_first_of(_WHITESPACES));
	if (_isServerDirective(directiveName) == false)
		throw std::invalid_argument("Bad directive name in server block\n");
	if (directiveName == "server_name")
		_parseServerNames(tLine, vs);
	if (directiveName == "listen")
		_parseListen(tLine, vs);
	if (directiveName == "client_max_body")
		_parseClientMaxBodySize(tLine, vs);
	if (directiveName == "error_page")
		_parseErrorPage(tLine, vs);
	if (directiveName == "index")
		_parseIndex(tLine, vs);
	if (directiveName == "root")
		_parseRoot(tLine, vs);
	if (directiveName == "autoindex")
		_parseAutoIndex(tLine, vs);
	if (directiveName == "return")
		_parseReturn(tLine, vs);
	if (directiveName == "cgi_allowed_ext")
		_parseCgiAllowed(tLine, vs);
	if (directiveName == "upload_path")
		_parseUploadPath(tLine, vs);
	if (directiveName == "methods")
		_parseMethods(tLine, vs);
}

// location overload
void	ConfigParser::_parseDirective(std::string const& line, Location& location) {
	(void) location;
	std::string	tLine = trim(line);
	std::string	directiveName = tLine.substr(0, tLine.find_first_of(_WHITESPACES));
	if (_isLocationDirective(tLine.substr(0, tLine.find_first_of(_WHITESPACES))) == false)
		throw std::invalid_argument("Bad directive name in location block\n");
	if (directiveName == "error_page")
		_parseErrorPage(tLine, location);
	if (directiveName == "index")
		_parseIndex(tLine, location);
	if (directiveName == "root")
		_parseRoot(tLine, location);
	if (directiveName == "autoindex")
		_parseAutoIndex(tLine, location);
	if (directiveName == "return")
		_parseReturn(tLine, location);
	if (directiveName == "cgi_allowed_ext")
		_parseCgiAllowed(tLine, location);
	if (directiveName == "upload_path")
		_parseUploadPath(tLine, location);
	if (directiveName == "methods")
		_parseMethods(tLine, location);
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
std::string	ConfigParser::_getServerBlock(std::stringstream& sContent) const {
	std::string					line;
	std::string					ret;
	std::vector<std::string>	vLine;
	bool						inBlock = false;

	std::getline(sContent, line);
	vLine = _splitInVector(line);
	if (vLine.size() != 2 || vLine.at(0) != "server" || vLine.at(1) != "{")
			throw std::invalid_argument("First line of block is bad in _getServerBlock()\n");
	ret += line + "\n";
	// stop condition for iterate on the function
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

std::string	ConfigParser::_getLocationBlock(std::stringstream& sContent) const {
	std::string					line;
	std::string					ret;
	std::vector<std::string>	vLine;

	std::getline(sContent, line);
	vLine = _splitInVector(line);
	if (vLine.size() != 3 || vLine.at(0) != "location" || vLine.at(2) != "{")
			throw std::invalid_argument("First line of block is bad in _getLocationBlock()\n");
	ret += line + "\n";
	while (std::getline(sContent, line)) {
		if (trim(line).empty() == true)
			continue ;
		ret += line + "\n";
		if (line.find("{") != std::string::npos)
			throw std::invalid_argument("No block can be in location block _getLocationBlock()\n");
		if (trim(line) == "}")
			return ret;
	}
	throw std::invalid_argument("Block is not closed in _getLocationBlock()\n");
}

void	ConfigParser::_parseFileContent(std::string const& fileContent) {
	std::stringstream			sContent(fileContent);
	std::string					line;
	std::vector<std::string>	vLine;

	while (std::getline(sContent, line)) {
		vLine = _splitInVector(line);
		if (vLine.empty() == true)
			continue ;
		if (vLine.size() != 2)
			throw std::invalid_argument("Bad syntax for server block in _parseFileContent()\n");
		if (vLine.at(0) == "server" && vLine.at(1) == "{") {
			for (size_t i = 0; i < line.size() + 1; i++)
				sContent.unget();
			_virtualServerList.push_back(_parseServerBlock(_getServerBlock(sContent)));
		}
		else
			throw std::invalid_argument("Bad syntax for server block in _parseFileContent()\n");
	}
}

VirtualServer	ConfigParser::_parseServerBlock(std::string const& serverBlock) {
	VirtualServer				vs;
	std::string					line;
	std::stringstream			sBlock(serverBlock);
	std::vector<std::string>	vLine;

	// skip first line
	std::getline(sBlock, line);
	while (std::getline(sBlock, line)) {
		vLine = _splitInVector(line);
		if (vLine.empty() == true)
			continue ;
		if (vLine.at(0) == "}") {
			vs.setDefaultValueToLocation();
			return vs;
		}
		if (vLine.at(0) == "location") {
			// back from last line to get the line "location sdfasdf {"
			for (size_t i = 0; i < line.size() + 1; i++)
				sBlock.unget();
			vs.setNewLocation(_parseLocationBlock(_getLocationBlock(sBlock)));
		} else
			_parseDirective(line, vs);
	}
	throw std::invalid_argument("Block is not closed in _parseServerBlock()\n");
}

Location	ConfigParser::_parseLocationBlock(std::string const& locationBlock) {
	Location					location;
	std::string					line;
	std::stringstream			sBlock(locationBlock);
	std::vector<std::string>	vLine;

	std::getline(sBlock, line);
	vLine = _splitInVector(line);
	location.setPath(vLine.at(1));
	while (std::getline(sBlock, line)) {
		vLine = _splitInVector(line);
		if (vLine.empty() == true)
			continue ;
		if (vLine.at(0) == "}")
			return location;
		_parseDirective(line, location);
	}
	throw std::invalid_argument("Block is not closed in _parseLocationBlock()\n");
}

std::vector<std::string>	ConfigParser::_splitInVector(std::string const& line) const {
	std::string					str = trim(line);
	std::vector<std::string>	v;
	size_t						nextSpace;

	if (str.empty() == true)
		return v;
	while (1) {
		nextSpace = str.find_first_of(_WHITESPACES);
		if (nextSpace == std::string::npos) {
			v.push_back(str);
			return v;
		}
		v.push_back(trim(str.substr(0, nextSpace)));
		str = trim(str.substr(nextSpace));
	}
}
