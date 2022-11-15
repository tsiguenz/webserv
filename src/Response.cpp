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
	// printResponse();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response(){
}


/*
** --------------------------------- MAIN METHODS ----------------------------------
*/

char *ft_strcpy(char *dest, const char *src){
	int i = 0;
	for (; src[i]; i++) {
		dest[i] = src[i];
	}
	dest[i] = src[i];
	return (dest);
}

char **mtoss(std::map<std::string, std::string> map) {
	char **strs;
	std::string str;
	if (!(strs = (char **)malloc(sizeof(char *) * (map.size() + 1))))
		return (NULL);
	int	i = -1;
	for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it){
		++i;
		// std::cout << "first " << (*it).first << std::endl
		// 		  << "second " << "=" << std::endl
		// 		  << "third " << map[(*it).first] << std::endl
		// 		  << "bob " << ((*it).first + "=" + map[(*it).first]).c_str() << std::endl
		// 		  << "billy " << (((*it).first + "=" + map[(*it).first]).c_str()) << std::endl
		// 		  << "bobby " << const_cast<char *>(((*it).first + "=" + map[(*it).first]).c_str()) << std::endl;
		str = (*it).first + "=" + map[(*it).first];
		if (!(strs[i] = (char *)malloc(sizeof(char) * (str.length() + 1))))
			return (NULL);
		strs[i] = ft_strcpy(strs[i], str.c_str());
	}
	strs[++i] = NULL;
	return (strs);
}

void				Response::buildingResponse(void) {

	if (code == 200)
		checkingMethod();
	
    if (code == 200 && (method == "GET" || method == "DELETE"))
	    getFile();

	Cgi bob(*this);
	std::map<std::string, std::string> map = bob.create_env();
	std::cout << RED "Printing map : " << std::endl;
	for(std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
		std::cout << CYAN << (*it).first << " : " WHITE << map[(*it).first] << std::endl;
	std::cout << std::endl << WHITE;

	char **envp = mtoss(map);
	for (int i = 0; envp && envp[i]; i++)
		std::cout << envp[i] << std::endl;
	// bob.executeCGI(fileName, )

	if (code == 200 && method == "DELETE") {

		deleteFile();
	}
	//et la
	if (code != 200)  {
		handleError();
	}

	response = getResponse();
	response += getTime();
	response += getServerName();
	response += getConnectionType();
	//redirect
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
	// std::list<std::string> allowedMethods = server.getAllowedMethods(); //server.
	// for (std::list<std::string>::iterator itVec = allowedMethods.begin(); itVec != allowedMethods.end(); itVec++)
	// {
	// 	if (method == (*itVec)) {
	// 		return ;
	// 	}			
	// }
	if (server.isAllowedMethod(method))
		return ;
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
	// std::cout << "c bon igo lurl mene a qlq chose" << std::endl; // DEBUG
	fileName = url;
	//check if its GET and if its in accepted
	return ;

}

void		Response::redirectionUrl(void) {
	if (url == "/")
		url = url + server.getIndex(); //si pas dindex renvoie sooit autoindex soit erreur
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

void	Response::printResponse(void) {
	std::cout << RED "fileName : " WHITE << fileName << std::endl
			  << CYAN "CONTENT_TYPE : " WHITE << mime.getMediaType(fileName) << std::endl
			  << CYAN "httpVersion : " WHITE << httpVersion << std::endl
			  << CYAN "method : " WHITE << method << std::endl
			  << CYAN "port : " WHITE << port << std::endl
			  << CYAN "serverName : " WHITE << serverName << std::endl
			  << RED "root : " WHITE << root << std::endl
			  << RED "url : " WHITE << url << std::endl;
	for(std::map<std::string, std::string>::iterator it = fieldLines.begin(); it != fieldLines.end(); ++it)
		std::cout << CYAN << (*it).first << " : " WHITE << fieldLines[(*it).first] << std::endl;
	std::cout << std::endl;
}
/* ************************************************************************** */
