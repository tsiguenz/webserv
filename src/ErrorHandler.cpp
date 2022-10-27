#include "ErrorHandler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ErrorHandler::ErrorHandler() {
	templateError = "<!DOCTYPE html><html><head><title></title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\"></p></body></html>";
	
	errorMessage[400] = "The server cannot or will not process the request due to something that is perceived to be a client error (e.g., malformed request syntax, invalid request message framing, or deceptive request routing)."; 
	errorMessage[401] = "The client must authenticate itself to get the requested response.";
	errorMessage[403] = "The client does not have access rights to the content.";
	errorMessage[404] = "The server cannot find the requested resource. In the browser, this means the URL is not recognized.";
	errorMessage[405] = "The request method is known by the server but is not supported by the target resource.";
	errorMessage[406] = "The server cannot produce a response matching the list of acceptable values defined in the request's proactive content negotiation headers.";
	errorMessage[408] = "TIME OUT";
	errorMessage[410] = "The requested content has been permanently deleted from server, with no forwarding address.";
	errorMessage[411] = "Server rejected the request because the Content-Length header field is not defined and the server requires it.";
	errorMessage[414] = "The URI requested by the client is longer than the server is willing to interpret.";
	errorMessage[415] = "The media format of the requested data is not supported by the server."; 
	errorMessage[431] = "The server is unwilling to process the request because its header fields are too large. The request may be resubmitted after reducing the size of the request header fields."; 
	errorMessage[500] = "The server has encountered a situation it does not know how to handle."; 
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

std::string	ErrorHandler::generateErrorHtml(int errorCode) {
	std::string templateGenerated(templateError);
	std::ostringstream s;
	s << errorCode;
	templateGenerated.insert(34, s.str());
	templateGenerated.insert(199, s.str());
	templateGenerated.insert(285, errorMessage[errorCode]);
	return(templateGenerated);
}