#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(void): isParsed(false), badRequest(false), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0")
{

}

Request::Request(std::string const & toParse): rawRequest(toParse), isParsed(true), badRequest(false), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0")
{
	std::cout << rawRequest;
	parsingRequest();
	printRequest();
	std::cout << rawRequest << std::endl;
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
	this->isParsed = rhs.isParsed;
	this->body = rhs.body;
	this->badRequest = rhs.badRequest;
	this->isParsed = rhs.isParsed;

	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


void	Request::parsingRequest(void) {

	isParsed = true;
	if (parsingRequestLine())
		return ;
	if (parsingFieldLines())
		return ;
	if (parsingBody())
		return;
	// if (checkingFile())
	// 	return;
}


int	Request::parsingRequestLine(void) { // [RFC]request-line   = method SP request-target SP HTTP-version CRLF

	std::string	firstLine = rawRequest.substr(0, rawRequest.find_first_of('\n') + 1);
	std::size_t nextSpace = firstLine.find_first_of(' ');

	if (nextSpace == std::string::npos){
		badRequest = true;
		return 1;
	}
	
	method = firstLine.substr(0, nextSpace);

	if (method != "GET" && method != "DELETE" && method != "POST"){
		badRequest = true;
		return 1;
	}

	firstLine = firstLine.erase(0, nextSpace + 1);
	nextSpace = firstLine.find_first_of(' ');

	if (nextSpace == std::string::npos){
		badRequest = true;
		return 1;
	}

	url =  firstLine.substr(0, nextSpace);

	if (url.find(illegalCharacter) != std::string::npos) {
		badRequest = true;
		return 1;
	}
	
	firstLine = firstLine.erase(0, nextSpace + 1);

	if (firstLine != "HTTP/1.1\r\n" && firstLine != "HTTP/1.0\r\n" && firstLine != "HTTP/0.9\r\n"){
		badRequest = true;
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
			badRequest = true;
			return 1;
	}	
	line = copyRequest.substr(0, copyRequest.find_first_of('\n') + 1);	
	while (!line.empty() && line != "\r\n") {
		
		// std::cout << "line= " << line << std::endl;
		if (line.find_first_of(':') == std::string::npos) {
			badRequest = true;
			return 1;
		}
		
		fieldName = line.substr(0, line.find_first_of(':'));
		line.erase(0, line.find_first_of(':') + 1);
		if (parsingFieldName(fieldName)) {
			badRequest = true;
			return 1;
		}
		fieldValue = parsingFieldValue(line.substr(0, line.find_first_of('\n') + 1));
		if (fieldValue.empty()) {
			badRequest = true;
			return 1;
		}
		copyRequest = copyRequest.erase(0, copyRequest.find_first_of('\n') + 1);
		if (copyRequest.find_first_of('\n') == std::string::npos) {
			badRequest = true;
			return 1;
		}	
		fieldLines[fieldName] = fieldValue;
		// std::cout << "fieldName= " << fieldName << " fieldValue= " << fieldValue << std::endl;
		line = copyRequest.substr(0, copyRequest.find_first_of('\n') + 1);
	}

	return 0;
}

std::string	Request::parsingFieldValue(std::string fieldValue) {
	return fieldValue;
}

int	Request::parsingFieldName(std::string fieldName) {
	if (fieldName.length() == 0 || fieldName.find_first_of(" \a\b\f\n\r\t\v\n") != std::string::npos)
		return 1;
	return 0;
}

int	Request::parsingBody(void) {
	if (rawRequest.find("\r\n\r\n", 4) == std::string::npos) {
			badRequest = true;
			return 1;
	}
	body = rawRequest.substr(rawRequest.find("\r\n\r\n") + 4);
	return 0;
}





















int					Request::checkingFile(void) {

	//en attendant le fichier de config
	std::string pathRepertoire = "/mnt/nfs/homes/aboudjel/Desktop/webserv/html";
	DIR *repertoire = opendir(pathRepertoire.c_str());
	if (!repertoire)
	{
		std::cout << "ERREUR: en attendant le fichier config jai mis le path des fichier dans /mnt/nfs/homes/aboudjel/Desktop/webserv/html faut mettre ta racine a toi dans \n";
		std::cout << "Request::checkingFile!" << std::endl;
	}
	closedir(repertoire);
	//
	if (url == "/")
		url = "/index.html";
	std::string path = pathRepertoire + url;
	FILE* fp = std::fopen(path.c_str(), "r"); //DEFINIR LE PATH PAR LE REPERTOIRE DANS CONFIG !!!!!!!
    if(!fp) {
        code = 404;
		std::cout << "404 mon PETIT REUF\n" << std::endl;
        return 1;
    }
	std::fclose(fp);
	std::cout << "c bon igo" << std::endl;
	return 0;

}








void				Request::create(std::string const & toParse) {
	rawRequest 	= toParse;
	isParsed 	= false;
	badRequest 	= false;
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