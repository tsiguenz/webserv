#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(std::string const & toParse): rawRequest(toParse), illegalCharacter("{}|\\^~[]` ")
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
	nextSpace = firstLine.find_first_of('\r');

	if (firstLine != "HTTP/1.1\r\n" && firstLine != "HTTP/1.0\r\n" && firstLine != "HTTP/0.9\r\n"){
		badRequest = true;
		return 1;
	}

	httpVersion = firstLine.substr(0, nextSpace);;

	return 0;
}
int	Request::parsingFieldLines(void) { // [RFC] header-field   = field-name ":" OWS field-value OWS  (OWS SP/HT)
}

/* ************************************************************************** */