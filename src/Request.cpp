#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(std::string const & toParse): rawRequest(toParse), badRequest(false), illegalCharacter("{}|\\^~[]` "), escapingCharacter("\a\b\f\n\r\t\v\'\"\\\0")
{
	parsingRequest();
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



// std::ostream &			operator<<( std::ostream & o, Request const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/


void	Request::parsingRequest(void) {

	if (parsingRequestLine())
		return ;
	if (parsingFieldLines())
		return ;
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




std::string	Request::parsingFieldValue(std::string fieldValue) {
	return fieldValue;
}





int	Request::parsingFieldName(std::string fieldName) {
	if (fieldName.length() == 0 || fieldName.find_first_of(" \a\b\f\n\r\t\v\n") != std::string::npos)
		return 1;
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
		copyRequest = rawRequest.erase(0, rawRequest.find_first_of('\n') + 1);
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






void	Request::printRequest(){

	std::cout << BGREEN << method << " " BCYAN << url <<  " " BRED <<  httpVersion << std::endl << std::endl;

	for(std::map<std::string, std::string>::iterator it = fieldLines.begin(); it != fieldLines.end(); it++) {
        std::cout << BGREEN << it->first << ": " BPURPLE << it->second << WHITE << std::endl;
    }
}
/* ************************************************************************** */