#include "ErrorHandler.hpp"
#include "webserv.h"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ErrorHandler::ErrorHandler() {
	
	templateError = "<!DOCTYPE html><html><head><title></title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\"></p></body></html>";
	errorMessage[200] = "OK";
	errorMessage[400] = "The server cannot or will not process the request due to something that is perceived to be a client error (e.g., malformed request syntax, invalid request message framing, or deceptive request routing)."; 
	errorMessage[401] = "The client must authenticate itself to get the requested response.";
	errorMessage[403] = "The client does not have access rights to the content.";
	errorMessage[404] = "The server cannot find the requested resource. In the browser, this means the URL is not recognized.";
	errorMessage[405] = "The request method is known by the server but is not supported by the target resource.";
	errorMessage[406] = "The server cannot produce a response matching the list of acceptable values defined in the request's proactive content negotiation headers.";
	errorMessage[408] = "TIME OUT";
	errorMessage[410] = "The requested content has been permanently deleted from server, with no forwarding address.";
	errorMessage[411] = "Server rejected the request because the Content-Length header field is not defined and the server requires it.";
	errorMessage[413] = "Request body is larger than limits defined by server.";
	errorMessage[414] = "The URI requested by the client is longer than the server is willing to interpret.";
	errorMessage[415] = "The media format of the requested data is not supported by the server."; 
	errorMessage[431] = "The server is unwilling to process the request because its header fields are too large. The request may be resubmitted after reducing the size of the request header fields."; 
	errorMessage[500] = "The server has encountered a situation it does not know how to handle.";


	statusCodes[200] = "OK";
	statusCodes[201] = "Created";
	statusCodes[202] = "Accepted";
	statusCodes[204] = "No Content";
	
	statusCodes[300] = "Multiple Choices";
	statusCodes[301] = "Moved Permanently";
	statusCodes[302] = "Found";
	statusCodes[304] = "Not Modified";
	
	statusCodes[400] = "Bad Request";
	statusCodes[401] = "Unauthorized";
	statusCodes[403] = "Forbidden";
	statusCodes[404] = "Not Found";
	statusCodes[405] = "Method Not Allowed";
	statusCodes[406] = "Not Acceptable";
	statusCodes[408] = "Request Timeout";
	statusCodes[410] = "Gone";
	statusCodes[411] = "Length Required";
	statusCodes[413] = "Payload Too Large";
	statusCodes[414] = "URI Too Long";
	statusCodes[415] = "Unsupported Media Type";
	statusCodes[431] = "Request Header Fields Too large";

	statusCodes[500] = "Internal Server Error";
}
//34
//195
//278
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ErrorHandler::~ErrorHandler()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::string ErrorHandler::generateStatusMessage(int errorCode) const {
	std::string msg = statusCodes.at(errorCode);
	return(msg);
}
std::string ErrorHandler::generateErrorHtml(int errorCode) const{

	std::string templateGenerated(templateError);
	std::ostringstream s;
	s << errorCode;
	templateGenerated.insert(34, s.str());
	templateGenerated.insert(199, s.str());
	templateGenerated.insert(285, errorMessage.at(errorCode));
	return(templateGenerated);
}
