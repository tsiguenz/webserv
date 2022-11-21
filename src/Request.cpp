#include "Request.hpp"
#include "webserv.h"
#include <stdio.h>
#include <stdlib.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(std::list<VirtualServer>	 & virtualServerList): _virtualServerList(virtualServerList), parsingCode(200), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0") {
	isRequestComplete = false;
	isParsingComplete = false;
}

Request::Request(): _virtualServerList(), parsingCode(200), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0") {
	isRequestComplete = false;
	isParsingComplete = false;
}
Request::Request(std::vector<unsigned char> & toParse):  vectorRequest(toParse), isRequestComplete(false), isParsingComplete(false), parsingCode(200), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0")
{
	parsingRequest();
//	printRequest();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}


/*
** --------------------------------- AddingBuffer ---------------------------------
*/

Request &				Request::operator=( Request const & rhs )
{
	this->_virtualServerList = rhs._virtualServerList;
	this->rawRequest = rhs.rawRequest;
	this->method = rhs.method;
	this->url = rhs.url;
	this->httpVersion = rhs.httpVersion;
	this->fieldLines = rhs.fieldLines;
	this->body = rhs.body;
	this->parsingCode = rhs.parsingCode;
	this->isParsingComplete = rhs.isParsingComplete;
	this->isRequestComplete = rhs.isRequestComplete;

	return *this;
}

void	Request::addingBuffer(std::vector<unsigned char> toAdd, ssize_t const& len) {
	vectorRequest.insert(vectorRequest.end(), toAdd.begin(), toAdd.begin() + len);
	parsingRequest();
}


// /*
// ** --------------------------------- METHODS ----------------------------------
// */

bool	Request::preParsing(void) {
	rawRequest = std::string(vectorRequest.begin(), vectorRequest.end());
	posEnd = rawRequest.find("\r\n\r\n", 4);
	if (posEnd == std::string::npos) {
			parsingCode = 400;
			return true;
	}
	else
		posEnd = posEnd + 4;
	return false;
}

void	Request::parsingRequest(void) {
	if (isParsingComplete == false) {
		if (preParsing()) {
			isParsingComplete = true;
			isRequestComplete = true;
			return ;
		}
		if (parsingRequestLine()) {
			isParsingComplete = true;
			isRequestComplete = true;
			return ;
		}
		if (parsingFieldLines()) {
			isParsingComplete = true;
			isRequestComplete = true;
			return ;
		}
		definingServer();
		if (definingBody()) {
			isParsingComplete = true;
			isRequestComplete = true;
			return ;
		}
		isParsingComplete = true;
	}
	if ( vectorRequest.size() >= requestLen)
		isRequestComplete = true;
	if (isRequestComplete == true) {
		if (parsingBody())
			return ;
	}

}

void Request::completingBody(void) {
	if (vectorRequest.size() >= requestLen)
		isRequestComplete = true;
}

int	Request::parsingRequestLine(void) { // [RFC]request-line   = method SP request-target SP HTTP-version CRLF

	std::string	firstLine = rawRequest.substr(0, rawRequest.find_first_of('\n') + 1);
	size_t		nextSpace = firstLine.find(" ");
	if (nextSpace == std::string::npos){
		parsingCode = 400;
		return 1;
	}
	method = firstLine.substr(0, nextSpace);
	firstLine = firstLine.erase(0, nextSpace + 1);
	nextSpace = firstLine.find_first_of(' ');
	if (nextSpace == std::string::npos){
		parsingCode = 400;
		return 1;
	}
	size_t		startQueryString = firstLine.find("?");
	if (startQueryString != std::string::npos) {
		size_t	endQueryString = firstLine.find("HTTP");
		if (endQueryString == std::string::npos) {
			parsingCode = 400;
			return 1;
		}
		queryString = firstLine.substr(startQueryString + 1, (endQueryString - 1) - (startQueryString + 1));
	}
	url = (startQueryString == std::string::npos) ? firstLine.substr(0, nextSpace) : firstLine.substr(0, startQueryString);
	if (url.find(illegalCharacter) != std::string::npos) {
		parsingCode = 400;
		return 1;
	}
	if (url.size() >= 2000) {
		parsingCode = 414;
		return 1;
	}
	firstLine = firstLine.erase(0, nextSpace + 1);
	if (firstLine != "HTTP/1.1\r\n" && firstLine != "HTTP/1.0\r\n" && firstLine != "HTTP/0.9\r\n"){
		parsingCode = 400;
		return 1;
	}
	nextSpace = firstLine.find_first_of('\r');
	httpVersion = firstLine.substr(0, nextSpace);;
	return 0;
}

int	Request::parsingFieldLines(void) { // [RFC] header-field   = field-name ":" OWS field-value OWS  (OWS =  SP/HT)

	std::string copyRequest = rawRequest.erase(0, rawRequest.find_first_of('\n') + 1);
	std::string line, fieldName, fieldValue;
	
	if (copyRequest.find_first_of('\n') == std::string::npos) {
		
			parsingCode = 400;
			return 1;
	}	
	line = copyRequest.substr(0, copyRequest.find_first_of('\n') + 1);	
	while (!line.empty() && line != "\r\n") {
		if (line.find_first_of(':') == std::string::npos || line.find("\r\n") == std::string::npos ) {
		
			parsingCode = 400;
			return 1;
		}
		
		fieldName = line.substr(0, line.find_first_of(':'));
		line.erase(0, line.find_first_of(':') + 1);
		if (parsingFieldName(fieldName)) {
		
			parsingCode = 400;
			return 1;
		}
		fieldValue = parsingFieldValue(line.substr(0, line.find_first_of('\n') + 1));
		if (fieldValue.empty()) {
		
			parsingCode = 400;
			return 1;
		}
		copyRequest = copyRequest.erase(0, copyRequest.find_first_of('\n') + 1);
		if (copyRequest.find_first_of('\n') == std::string::npos) { // \n\r???
		
			parsingCode = 400;
			return 1;
		}	
		fieldLines[fieldName] = fieldValue;
		line = copyRequest.substr(0, copyRequest.find_first_of('\n') + 1);
	}
	trimingFieldLines();
	return 0;
}

std::string	Request::parsingFieldValue(std::string fieldValue) { //si du parsing a faire sur la fieldValue la metttre ici
	return fieldValue;
}

int	Request::parsingFieldName(std::string fieldName) {
	if (fieldName.length() == 0 || fieldName.find_first_of(" \a\b\f\n\r\t\v\n") != std::string::npos)
		return 1;
	return 0;
}

void	Request::definingServer() {
	server = _getVirtualServerByHost();
}
int	Request::definingBody() {

	std::map<std::string,std::string>::iterator it;
	it = fieldLines.find("Content-Length");

  	if (it == fieldLines.end()) {
		isRequestComplete = true;
		if (method == "POST")
			parsingCode = 411;
		return 1;
	}
	int contentLenght = strtod(fieldLines["Content-Length"].c_str(), NULL) ;
	if (contentLenght > server.getClientMaxBodySize()) {
		parsingCode = 413;
		return 1;
	}
	requestLen = contentLenght + posEnd;
	return 0;
}


VirtualServer const 	Request::_selectServer(short const& port, std::string const& ip, std::string const& serverName) const {
	std::list<VirtualServer>	candidatVirtualServer = _virtualServerList;
	std::list<VirtualServer>::iterator it = candidatVirtualServer.begin();
	std::list<VirtualServer>::iterator end = candidatVirtualServer.end();
	std::list<VirtualServer>::iterator itcpy;

	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	while (it != end)
	{
		if (port != (*it).getPort()) {
			itcpy = it;
			it++;
			candidatVirtualServer.erase(itcpy);
		}
		else
			it++;
	}
	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	it = candidatVirtualServer.begin();
	while (it != end)
	{
		if (ip != (*it).getIp()) {
			itcpy = it;
			it++;
			candidatVirtualServer.erase(itcpy);
		}
		else
			it++;
	}
	if (candidatVirtualServer.size() == 1)
		return(candidatVirtualServer.front());
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	it = candidatVirtualServer.begin();
	if (serverName.empty())
		return(candidatVirtualServer.front());
	std::list<std::string>::iterator itList;
	for (; it != end; it++) {
		bool	isNamePresent = false;
		itList = (*it).getServerNames().begin();
		for (; itList != (*it).getServerNames().end(); itList++)
		{
			if (serverName == (*itList))
				isNamePresent = true;
		}
		if (isNamePresent == false) {
			if (candidatVirtualServer.size() == 1)
				break ;
			candidatVirtualServer.erase(it);
		}
	}
	if (candidatVirtualServer.empty())
		return(_virtualServerList.front());
	return(candidatVirtualServer.front());
}


VirtualServer const	Request::_getVirtualServerByHost() const {

	std::string	ip;
	std::string	portString;
	std::string	serverName;
	int			port = -1;

	std::map<std::string, std::string>::const_iterator itFields = fieldLines.find("Host");
	if (itFields == fieldLines.end())
		return(_virtualServerList.front());
	std::string	hostName = (*itFields).second;
	size_t	posLocalHost = hostName.find("localhost");
	if (posLocalHost != std::string::npos)
		hostName.replace(posLocalHost, 9, "127.0.0.1");
	size_t	posSeparator = hostName.find_last_of(':');
	if (posSeparator == std::string::npos) {
		if (is_digits(hostName) && hostName.size() < 6)
			port = atoi(hostName.c_str());
		else if (validateIP(hostName))
			ip = hostName;
		else
			serverName = hostName;
	}
	else {
		std::string	firstPart = hostName.substr(0, posSeparator);
		std::string	secondPart = hostName.substr(posSeparator + 1);
		if(validateIP(firstPart))
			ip = firstPart;
		else
			serverName =firstPart;
		if (is_digits(secondPart) && secondPart.size() < 6)
			port = atoi(secondPart.c_str());
		else
			return(_virtualServerList.front());
	}
	if (port > 65535)
		return(_virtualServerList.front());
	if (port == -1)
		port = 8080;
	if (ip.empty())
		ip = "0.0.0.0";
	return(_selectServer(port, ip, serverName));
}

int	Request::parsingBody(void) {
	body = std::vector<unsigned char>(vectorRequest.begin() + posEnd, vectorRequest.begin() + requestLen);
	return 0;
}

void	Request::trimingFieldLines() {
	
	for ( std::map<std::string, std::string>::iterator it = fieldLines.begin(); it != fieldLines.end(); it++)
	{
		(*it).second = trim((*it).second);
	}
	
}

void	Request::printRequest(){

//	std::cout << BGREEN << method << " " BCYAN << url <<  " " BRED <<  httpVersion << WHITE <<  std::endl << std::endl;
//
//	for(std::map<std::string, std::string>::iterator it = fieldLines.begin(); it != fieldLines.end(); it++) {
//        std::cout << BGREEN << it->first << ": " BPURPLE << it->second << WHITE << std::endl;
//    }
//
//	std::cout << BGREEN << "BODY: " << std::endl;
//	if (body.empty())
//		std::cout << BRED << "NONE" WHITE << std::endl;
//	else {
//		for (std::vector<unsigned char>::iterator it = body.begin(); it != body.end();it++){
//   			std::cout << (*it);
// 		}
//		std::cout << std::endl;
//	}
	std::cout << "--------------RAWREQUEST----------------"<< std::endl;
	for (std::vector<unsigned char>::iterator it = vectorRequest.begin(); it != vectorRequest.end();it++){
   			std::cout << (*it);
 		}
		std::cout << std::endl;
}
/* ************************************************************************** */
