#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//: mime()
Response::Response( Request  src ): mime(){

	if (!src.isParsed)
		code = 500;
	else if (src.badRequest)
		code = 400;
    else if (src.uriTooLong)
        code = 414;
	else {
		code = 200;
		method = src.method;
		url = src.url;
		httpVersion = src.httpVersion;
		fieldLines = src.fieldLines;
		body = src.body;
	}
	initMapCode();
	buildingResponse();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response(){
}


/*
** --------------------------------- MAIN METHODS ----------------------------------
*/

void				Response::buildingResponse(void) {


    if (code == 200 && method == "GET")
	    getFile();
	//if post ou delete 411

	
	
	if (code != 200)  {
		handleError();
	}

	response = getResponse();
	response += getTime();
	response += getServerName();
	if (!file.empty())
		response += getTypeContent();
	response += getLength();
	response += getConnectionType();
	response += "\r\n";
	if (!file.empty())
		response += std::string(file.begin(), file.end());
}

/*
** --------------------------------- GET METHODE ----------------------------------
*/

void		Response::getFile(void) {

	//en attendant le fichier de config
	std::string pathRepertoire = "html";
	DIR *repertoire = opendir(pathRepertoire.c_str());
	if (!repertoire)
	{
		std::cout << "ERREUR: en attendant le fichier config jai mis le path des fichier dans /mnt/nfs/homes/aboudjel/Desktop/webserv/html faut mettre ta racine a toi dans \n";
		std::cout << "Request::checkingFile!" << std::endl;
	}
	closedir(repertoire);

	redirectionUrl();
	try {
		mime.getMediaType(url);
	}
	catch(std::exception& e) {	
		code = 415;
		return;
	}
	std::string path = pathRepertoire + url;
	// FILE* fp = std::fopen(path.c_str(), "r"); //DEFINIR LE PATH PAR LE REPERTOIRE DANS CONFIG !!!!!!!
	std::ifstream file(path.c_str());
    if(file.fail()) {
        code = 404;
		std::cout << "404 mon PETIT REUF\n" << std::endl;
        return ;
    }
	this->file = std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
	file.close();
	std::cout << "c bon igo lurl mene a qlq chose" << std::endl;
	fileName = url;
	return ;

}

void		Response::redirectionUrl(void) {
	if (url == "/") //selon fichier de config
		url = "/index.html";
}

/*
** --------------------------------- HANDLE ERROR ----------------------------------
*/

void		Response::handleError(void) {
	if (code == 100) {
		file.clear();
	}

	if (code > 399) {
		fileName = "error.html";
		file = std::vector<char>(statusCodes[code].second.begin(), statusCodes[code].second.end());
	}
}


/*
** ------------------------ SETTING UP EACH RESPONSE LINE -------------------------
*/

std::string	Response::getTime(void) {

	std::string line = "Date: ";
 	std::time_t result = std::time(NULL);
    line += std::asctime(std::localtime(&result));
	line.insert(line.find('\n'), " GMT\r");
	return (line);
}

std::string Response::getServerName(void) {
    std::string line = "Server: WebServ 1.0 \n\r";
    return line;
}

std::string Response::getLength(void) {
	std::string line = "Content-Length: ";
	std::ostringstream s;
	s << std::distance(this->file.begin(), this->file.end());
	line += std::string(s.str());
	line += "\r\n";
	return line;
}

std::string Response::getTypeContent(void) {
	std::string line = "Content-Type: "; //
	std::string contentType = this->mime.getMediaType(fileName); //fileName qui contient le name de ce quon envoie
	line += contentType;
	if (contentType == "text/html"){
		line += "; charset=utf-8";
	}
	line += "\r\n";
	return line;
}

std::string Response::getConnectionType(void) { //pour linstant close, a voir si on gere le keep alive
	std::string line = "Connection: close";
	line += "\r\n";
	return line;
}

std::string Response::getResponse(void) {
	std::string line = "HTTP/1.1 ";
	std::ostringstream s;
	s << code;
	line += std::string(s.str());
	line += " ";
	line += statusCodes[code].first;
	line += "\r\n";
	return line;
}





void	Response::initMapCode(void) {

	statusCodes.insert( std::make_pair( 100, std::make_pair("Continue","empty")));

	statusCodes.insert( std::make_pair( 200, std::make_pair("OK","empty")));
	statusCodes.insert( std::make_pair( 201, std::make_pair("Created","empty")));
	statusCodes.insert( std::make_pair( 202, std::make_pair("Accepted","empty")));
	statusCodes.insert( std::make_pair( 204, std::make_pair("No Content","empty")));

	statusCodes.insert( std::make_pair( 300, std::make_pair("Multiple Choices","empty")));
	statusCodes.insert( std::make_pair( 301, std::make_pair("Moved Permanently","empty")));
	statusCodes.insert( std::make_pair( 302, std::make_pair("Found","empty")));
	statusCodes.insert( std::make_pair( 304, std::make_pair("Not Modified","empty")));

	statusCodes.insert( std::make_pair( 400, std::make_pair("Bad Request","<!DOCTYPE html><html><head><title>400</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/400\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The server cannot or will not process the request due to something that is perceived to be a client error (e.g., malformed request syntax, invalid request message framing, or deceptive request routing).</p></body></html>")));
	statusCodes.insert( std::make_pair( 401, std::make_pair("Unauthorized","<!DOCTYPE html><html><head><title>401</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/401\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The client must authenticate itself to get the requested response.</p></body></html>")));
	statusCodes.insert( std::make_pair( 403, std::make_pair("Forbidden","<!DOCTYPE html><html><head><title>403</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/403\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The client does not have access rights to the content.</p></body></html>")));
	statusCodes.insert( std::make_pair( 404, std::make_pair("Not Found","<!DOCTYPE html><html><head><title>404</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/404\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The server cannot find the requested resource. In the browser, this means the URL is not recognized. </p></body></html>")));
	statusCodes.insert( std::make_pair( 405, std::make_pair("Method Not Allowed","<!DOCTYPE html><html><head><title>405</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/405\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The request method is known by the server but is not supported by the target resource.</p></body></html>")));
	statusCodes.insert( std::make_pair( 408, std::make_pair("Request Timeout","<!DOCTYPE html><html><head><title>408</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/408\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">TIME OUT</p></body></html>")));
	statusCodes.insert( std::make_pair( 410, std::make_pair("Gone","<!DOCTYPE html><html><head><title>410</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/410\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The requested content has been permanently deleted from server, with no forwarding address.</p></body></html>")));
	statusCodes.insert( std::make_pair( 411, std::make_pair("Length Required","<!DOCTYPE html><html><head><title>411</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/411\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">Server rejected the request because the Content-Length header field is not defined and the server requires it.</p></body></html>")));
	statusCodes.insert( std::make_pair( 414, std::make_pair("URI Too Long","<!DOCTYPE html><html><head><title>414</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/414\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The URI requested by the client is longer than the server is willing to interpret.</p></body></html>")));
	statusCodes.insert( std::make_pair( 415, std::make_pair("Unsupported Media Type","<!DOCTYPE html><html><head><title>415</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/415\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The media format of the requested data is not supported by the server.</p></body></html>")));
	statusCodes.insert( std::make_pair( 431, std::make_pair("Request Header Fields Too large","<!DOCTYPE html><html><head><title>431</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/431\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The server is unwilling to process the request because its header fields are too large. The request may be resubmitted after reducing the size of the request header fields.</p></body></html>")));
	statusCodes.insert( std::make_pair( 500, std::make_pair("Internal Server Error","<!DOCTYPE html><html><head><title>500</title></head><body style=\"background-color: rgb(0, 0, 0);\"><img style=\"display: block; margin-left: auto; margin-right: auto; width: 50%\" src=\"https://http.cat/500\"><p style=\"text-align: center; margin-left: 30%; margin-right: 30%; color: white\">The server has encountered a situation it does not know how to handle.</p></body></html>")));

	// statusCodes[200] = "OK";//
	// statusCodes[201] = "Created";
	// statusCodes[202] = "Accepted";
	// statusCodes[204] = "No Content";
	
	// statusCodes[300] = "Multiple Choices";
	// statusCodes[301] = "Moved Permanently";
	// statusCodes[302] = "Found";
	// statusCodes[304] = "Not Modified";
	
	// statusCodes[400] = "Bad Request"; //
	// statusCodes[401] = "Unauthorized";
	// statusCodes[403] = "Forbidden";//
	// statusCodes[404] = "Not Found";
	// statusCodes[405] = "Method Not Allowed";
	// statusCodes[408] = "Request Timeout";//
	// statusCodes[410] = "Gone";
	// statusCodes[411] = "Length Required";//
	// statusCodes[414] = "URI Too Long";//
	// statusCodes[415] = "Unsupported Media Type"; //
	// statusCodes[431] = "Request Header Fields Too large"; //

	// statusCodes[500] = "IInternal Server Error"; //

}

void	Response::printResponse(void) const {
	std::cout << response << std::endl;
}
/* ************************************************************************** */
