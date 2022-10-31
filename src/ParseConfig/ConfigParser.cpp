#include "ConfigParser.hpp"

// Object managment

ConfigParser::ConfigParser(): _fileName(), _fileContent(), _WHITESPACES(" \n\r\t\f\v") {
	_initAllowedDirectives();
	_virtualServerList.push_back(VirtualServer());
	VirtualServer&	vs = _virtualServerList.front();
	vs.setIp("0.0.0.0");
	vs.setPort(8080);
	vs.setClientMaxBodySize(1048576);
}

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

	while ((serverBlock = _getServerBlock(sContent)) != "")
		_parseServerBlock(serverBlock);
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
//		std::cout << line.substr(0, line.find_first_of(_WHITESPACES)) << std::endl;
	}
	return vs;
}

void	ConfigParser::_parseLocationBlock(std::string const& locationBlock) {
	(void) locationBlock;
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
	if (directiveName == "error_page")
		_parseErrorPage(tLine, vs);
	if (directiveName == "client_max_body")
		_parseClientMaxBodySize(tLine, vs);
	if (directiveName == "autoindex")
		_parseAutoIndex(tLine, vs);
	if (directiveName == "root")
		_parseRoot(tLine, vs);
	if (directiveName == "index")
		_parseIndex(tLine, vs);
	if (directiveName == "methods")
		_parseMethods(tLine, vs);
	if (directiveName == "return")
		_parseReturn(tLine, vs);
	if (directiveName == "cgi_allowed_ext")
		_parseCgiAllowed(tLine, vs);
	if (directiveName == "upload_path")
		_parseUploadPath(tLine, vs);
}

// location overload
void	ConfigParser::_parseDirective(std::string const& line, Location& location) {
	(void) location;
	std::string	tLine = trim(line);
	std::string	directiveName = tLine.substr(0, tLine.find_first_of(_WHITESPACES));
	if (_isLocationDirective(tLine.substr(0, tLine.find_first_of(_WHITESPACES))) == false)
		throw std::invalid_argument("Bad directive name in location block\n");
//	if (directiveName == "redir")
//		_parseRedir(tLine, location);
}

void	ConfigParser::_parseServerNames(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() < 2)
		throw std::invalid_argument("server_name directive bad number of arguments (ex: server_name webserv.com\n");
	for (std::vector<std::string>::const_iterator it = v.begin() + 1; it != v.end(); it++)
		vs.setServerName(*it);
}

void	ConfigParser::_parseListen(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 2)
		throw std::invalid_argument("listen directive bad number of arguments (ex: listen 127.0.0.1:8080)\n");
	std::string	str = v.back();
	size_t	sepPos = str.find(":");
	// only port case
	if (sepPos == std::string::npos && str.find(".") == std::string::npos)
		vs.setPort(std::strtod(str.c_str(), NULL));
	// only ip case
	if (sepPos == std::string::npos && str.find(".") != std::string::npos)
		vs.setIp(str);
	// port and ip case
	if (sepPos != std::string::npos) {
		vs.setIp(str.substr(0, sepPos));
		str = str.substr(sepPos + 1);
		vs.setPort(std::strtod(str.c_str(), NULL));
	}
}

void	ConfigParser::_parseErrorPage(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() < 3)
		throw std::invalid_argument("error_page directive number of arguments (ex: error_page 400 404 4xx.html)\n");
	v.erase(v.begin());
	std::string	path = v.back();
	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end() -1; it++)
		vs.setErrorPage(std::strtod((*it).c_str(), NULL), path);
}

void	ConfigParser::_parseClientMaxBodySize(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 2)
		throw std::invalid_argument("client_max_body directive bad syntax\n");
	int	maxBodySize = strtod(v.back().c_str(), NULL);
	// 10 MB limit
	if (maxBodySize <= 0 || maxBodySize > 1048576)
		throw std::invalid_argument("client_max_body bad value\n");
	vs.setClientMaxBodySize(maxBodySize);
}

void	ConfigParser::_parseAutoIndex(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 2)
		throw std::invalid_argument("autoindex directive bad number of arguments (ex: autoindex on)\n");
	bool	autoIndex;
	if (v.back() == "on")
		autoIndex = true;
	else if (v.back() == "off")
		autoIndex = false;
	else
		throw std::invalid_argument("autoindex directive bad argument (argument must be on or off\n");
	vs.setAutoIndex(autoIndex);
}

void	ConfigParser::_parseRoot(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 2)
		throw std::invalid_argument("root directive bad number of arguments (ex: root /var/www)\n");
	vs.setRoot(v.back());
}

void	ConfigParser::_parseIndex(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 2)
		throw std::invalid_argument("index directive number of arguments (ex: index index.html)\n");
	vs.setIndex(v.back());
}

void	ConfigParser::_parseMethods(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() == 1)
		throw std::invalid_argument("methods directive number of arguments (ex: methods GET POST DELETE\n");
	for (std::vector<std::string>::const_iterator it = v.begin() + 1; it != v.end(); it++) {
		if (*it == "GET" || *it == "POST" || *it == "DELETE")
				vs.setAllowedMethod(*it);
		else
			throw std::invalid_argument("methods directive bad argument (must be GET or/and POST or/and DELETE\n");
	}
}

void	ConfigParser::_parseReturn(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 3)
		throw std::invalid_argument("return directive bad number of arguments (ex: return 404 google.com)\n");
	int	code = std::strtod(v.at(1).c_str(), NULL);
	if (code == 0)
		throw std::invalid_argument("return directive bad code (ex: return 404 google.com)\n");
	vs.setReturn(code, v.at(2));
}

void	ConfigParser::_parseCgiAllowed(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() == 1)
		throw std::invalid_argument("cgi_allowed_ext directive bad number of arguments (ex: cgi_allowed_ext .py .php)\n");
	for (std::vector<std::string>::const_iterator it = v.begin() + 1; it != v.end(); it++) {
		if ((*it).at(0) == '.')
				vs.setAllowedExtCgi(*it);
		else
			throw std::invalid_argument("cgi_allowed_ext directive bad argument (must start by .)\n");
	}
}

void	ConfigParser::_parseUploadPath(std::string const& line, VirtualServer& vs) {
	std::vector<std::string>	v = _splitInVector(line);
	if (v.size() != 2)
		throw std::invalid_argument("upload_path directive bad number of arguments (ex: upload_path html/download)\n");
	vs.setUploadPath(v.back());
}

std::vector<std::string>	ConfigParser::_splitInVector(std::string const& line) const {
	std::string					str = line;
	std::vector<std::string>	v;
	size_t						nextSpace;

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
