#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(void): parsingCode(500), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0")
{
	parsingCode = 2;
}

Request::Request(std::string const & toParse): rawRequest(toParse), parsingCode(200), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0")
{
	std::cout << rawRequest; //DEBUG
	parsingRequest();
	printRequest(); //DEBUG
	// std::cout << rawRequest << std::endl; //DEBUG
}

Request::Request( const Request & src )
{
	*this = src;
}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/



Request &				Request::operator=( Request const & rhs )
{
	this->rawRequest = rhs.rawRequest;
	this->method = rhs.method;
	this->url = rhs.url;
	this->httpVersion = rhs.httpVersion;
	this->fieldLines = rhs.fieldLines;
	this->body = rhs.body;
	this->parsingCode = rhs.parsingCode;

	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


void	Request::parsingRequest(void) {

	if (parsingRequestLine())
		return ;
	if (parsingFieldLines())
		return ;
	if (parsingBody())
		return ;

	if (method == "POST") { // ----> config
		std::map<std::string,std::string>::iterator it;
		it = fieldLines.find("Content-Length");
  		if (it == fieldLines.end()) {	
			parsingCode = 411;
			return ;
		}
	}
}


int	Request::parsingRequestLine(void) { // [RFC]request-line   = method SP request-target SP HTTP-version CRLF

	std::string	firstLine = rawRequest.substr(0, rawRequest.find_first_of('\n') + 1);
	std::size_t nextSpace = firstLine.find_first_of(' ');

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

	url =  firstLine.substr(0, nextSpace);

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

		// std::cout << "line= " << line << std::endl; //DEBUG
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
		// std::cout << "fieldName= " << fieldName << " fieldValue= " << fieldValue << std::endl; //DEBUG
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

int	Request::parsingBody(void) {
	if (rawRequest.find("\r\n\r\n", 4) == std::string::npos) {
			parsingCode = 400;
			return 1;
	}
	body = rawRequest.substr(rawRequest.find("\r\n\r\n") + 4); //mettre en vector<unsigned char>
	return 0;
}

void	Request::trimingFieldLines() {
	
	for ( std::map<std::string, std::string>::iterator it = fieldLines.begin(); it != fieldLines.end(); it++)
	{
		(*it).second = trim((*it).second);
	}
	
}
void				Request::create(std::string const & toParse) {
	rawRequest 	= toParse;
	parsingCode = 200;
	parsingRequest();
}

void				Request::create(Request const & rhs) {
	*this = rhs;
}























void	Request::printRequest(){

	std::cout << BGREEN << method << " " BCYAN << url <<  " " BRED <<  httpVersion << WHITE <<  std::endl << std::endl;

	for(std::map<std::string, std::string>::iterator it = fieldLines.begin(); it != fieldLines.end(); it++) {
        std::cout << BGREEN << it->first << ": " BPURPLE << it->second << WHITE << std::endl;
    }

	std::cout << BGREEN << "BODY: ";
	if (body.empty())
		std::cout << BRED << "NONE" WHITE << std::endl;
	else
		std::cout << BCYAN << body << WHITE << std::endl;
}
/* ************************************************************************** */