#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//: mime()
Response::Response( Request  src, VirtualServer const & virtualServer ): mime(), server(virtualServer){

	code = src.parsingCode;
	method = src.method;
	url = src.url;
	httpVersion = src.httpVersion;
	fieldLines = src.fieldLines;
	body = src.body;
	root = server.getRoot();
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

	if (code == 200)
		checkingMethod();
    if (code == 200 && (method == "GET" || method == "DELETE"))
	    getFile();
	if (code == 200 && method == "DELETE") {

		deleteFile();
	}
	
	if (code != 200)  {
		handleError();
	}

	response = getResponse();
	response += getTime();
	response += getServerName();
	response += getConnectionType();
	if (method == "GET" || (code != 200 && (method == "DELETE" || method == "POST"))) {
		if (!file.empty())
			response += getTypeContent();
		response += getLength();
		response += "\r\n";
		if (!file.empty())
			response += std::string(file.begin(), file.end());
	}
	else
		response += "\r\n";
}

/*
** --------------------------------- GET METHODE ----------------------------------
*/
void		Response::checkingMethod(void) {
	std::list<std::string> allowedMethods = server.getAllowedMethods();
	for (std::list<std::string>::iterator itVec = allowedMethods.begin(); itVec != allowedMethods.end(); itVec++)
	{
		if (method == (*itVec)) {
			return ;
		}			
	}
	if (method == "DELETE" || method == "GET" || method == "POST" || method == "HEAD" || method == "PATCH" || method == "PUT" || method == "OPTIONS" || method == "CONNECT" || method == "TRACE") {
		code = 405;
		return ;
	}
	code = 400;
	return ;
}
void		Response::getFile(void) {

	std::string pathRepertoire = server.getRoot(); 
	redirectionUrl();
	// redirectionUrl(); // en faire un qui redir //SELON CONFIG
	// redirectionUrl(); // en faire un spé root //SELON CONFIG
	if (!mime.isExtensionSupported(url)) {	
		code = 415;
		return;
	}
	std::string path = pathRepertoire + url;
	std::ifstream file(path.c_str());
    if(file.fail()) {
        code = 404;
		std::cout << "404 mon PETIT REUF\n" << std::endl; // DEBUG
        return ;
    }
	this->file = std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
	file.close();
	std::cout << "c bon igo lurl mene a qlq chose" << std::endl; // DEBUG
	fileName = url;
	//check if its GET and if its in accepted
	return ;

}

void		Response::redirectionUrl(void) {
	if (url == "/")
		url = url + server.getIndex();
}


/*
** --------------------------------- DELETE METHODE ----------------------------------
*/

	void	Response::deleteFile(void) {
		std::string pathRepertoire = server.getRoot();

		if (fileName.find("..") != std::string::npos) {
			code = 400;
			return;
		}

		if (remove((pathRepertoire +fileName).c_str()) != 0) {
     		code = 403;
		}
	}

/*
** --------------------------------- HANDLE ERROR ----------------------------------
*/

void		Response::handleError(void) {
	if(server.getErrorPages().count(code)) {
		fileName = server.getRoot() + server.getErrorPageByCode(code);
		std::ifstream fileBuffer(fileName.c_str());
   		if(fileBuffer.fail()) {
        	fileName = "error.html";
			std::string fileGenerated = server.errorHandler.generateErrorHtml(code);
			file = std::vector<char>(fileGenerated.begin(), fileGenerated.end());
        	return ;
   		}
		this->file = std::vector<char>((std::istreambuf_iterator<char>(fileBuffer)),std::istreambuf_iterator<char>());
		fileBuffer.close();
		return ;
	}

	if (code > 399) {
		fileName = "error.html";
		std::string fileGenerated = server.errorHandler.generateErrorHtml(code);
		file = std::vector<char>(fileGenerated.begin(), fileGenerated.end());
        return ;
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
	std::string line = "Content-Type: ";
	std::string contentType = this->mime.getMediaType(fileName);
	line += contentType;
	if (contentType == "text/html"){
		line += "; charset=utf-8";
	}
	line += "\r\n";
	return line;
}

std::string Response::getConnectionType(void) {
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
	line += server.errorHandler.generateStatusMessage(code);
	line += "\r\n";
	return line;
}

void	Response::printResponse(void) const {
	std::cout << response << std::endl;
}
/* ************************************************************************** */
