#include "Response.hpp"
#include <iostream>
#include <sstream>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response()
	: method(), url(), httpVersion(), fieldLines(), body(), root(),
	serverName(), port(), fileName(), file(), response(), mime(), server(), statusCodes()
{ }

//: mime()
Response::Response( Request  src, VirtualServer const & virtualServer ): mime(), server(virtualServer){

	code = src.parsingCode;
	method = src.method;
	url = src.url;
	httpVersion = src.httpVersion;
	fieldLines = src.fieldLines;
	body = src.body;
	root = server.getRoot();
	serverName = server.getServerNames().front();
	std::stringstream ss;
	ss << server.getPort();
	port = ss.str();

	buildingResponse();
//	printResponse();
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
	//tu peux faire ton bail entre ici
    if (code == 200 && (method == "GET" || method == "DELETE"))
	    getFile();
	if (code == 200 && method == "DELETE")
		deleteFile();
	if (code == 200 && method == "POST")
		postFile();
	//et la
	if (code != 200)
		handleError();

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
//		std::cout << "404 mon PETIT REUF\n" << std::endl; // DEBUG
        return ;
    }
	this->file = std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
	file.close();
//	std::cout << "c bon igo lurl mene a qlq chose" << std::endl; // DEBUG
	fileName = url;
	//check if its GET and if its in accepted
	return ;

}

void		Response::redirectionUrl(void) {
	if (url == "/")
		url = url + server.getIndex();
}


/*
** --------------------------------- POST METHODE ----------------------------------
*/

void		Response::postFile(void) {
	for (std::vector<char>::iterator it = body.begin(); it != body.end(); it++)
		std::cout << *it;
	std::cout << std::endl;
	if (fieldLines["Content-Type"].find("multipart/form-data") == 0)
		_postFormData();
}

void	Response::_postFormData() {
	size_t	pos = fieldLines["Content-Type"].find("boundary=");
	if (pos == std::string::npos) {
		code = 400;
		return ;
	}
	for (std::vector<char> v = _getFormDataBlock(); v.empty() == false; v = _getFormDataBlock()) {
		if (code == 400)
			return ;
		_postFormDataBlock(v);
	}
}

void	Response::_postFormDataBlock(std::vector<char> const& v) {
	std::string	fileName;
	std::cout << "---------- Enter in _postBoundaryBlock() ----------\n";
	for (std::vector<char>::const_iterator it = v.begin(); it != v.end(); it++)
		std::cout << *it;
	std::cout << std::endl;
	size_t	pos = _itFind(v.begin(), v.end(), "filename=\"");
	std::cout << "pos = " << pos << std::endl;
	if (pos == std::string::npos) {
		code = 400;
		return ;
	}
	pos += 10;
	while (v[pos] != '"')
		fileName.push_back(v[pos++]);
	std::cout << "fileName = " << fileName << std::endl;
}

std::vector<char>	Response::_getFormDataBlock() {
	static size_t	pos = 0;
	std::vector<char>	ret;
	size_t	posDelim = fieldLines["Content-Type"].find("boundary=");
	if (posDelim == std::string::npos)
		return ret;
	std::string	delim = fieldLines["Content-Type"].substr(posDelim + 9);
	size_t	startBlock = _itFind(body.begin(), body.end(), delim, pos);
	size_t	endBlock = _itFind(body.begin(), body.end(), delim, startBlock + delim.size());
	pos = endBlock;
	if (startBlock == std::string::npos || endBlock == std::string::npos)
		return ret;
	for (; startBlock != endBlock; startBlock++)
		ret.push_back(body[startBlock]);
	return ret;
}

//size_t	Response::_itFind(It it, It end, std::string const& toSearch, size_t pos) {
//	if (pos > (size_t) std::distance(body.begin(), body.end()) || toSearch.empty() == true)
//		return std::string::npos;
//	for (std::vector<char>::iterator it = body.begin() + pos; it != body.end(); it++) {
//		if (*it != toSearch[0])
//			continue ;
//		for (size_t i = 0; i < toSearch.size(); i++) {
//			if (*(it + i) != toSearch[i])
//				break ;
//			if (i == toSearch.size() - 1)
//				return std::distance(body.begin(), it);
//		}
//	}
//	return std::string::npos;
//}

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
