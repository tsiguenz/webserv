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
Response::Response( Request  src, VirtualServer const & virtualServer ): mime(), server(virtualServer), isAutoIndex(false){

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
** --------------------------------- AUTO INDEX HTML ----------------------------------
*/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <fstream>

std::string readFile(std::string path)
{
	std::ifstream 	ifs;
	std::string		file_content;
	std::string		filename = path;
	int				length;
	char			*buff;


	ifs.open(filename.c_str(), std::ifstream::in);

	if ( !ifs.is_open() )
		return ("");

	ifs.seekg (0, ifs.end);
	length = ifs.tellg();
	ifs.seekg (0, ifs.beg);

	buff = new char[length + 1];
	ifs.read(buff, length);
	if (ifs.fail())
	{
        ifs.close();
        return ("");
    }
	ifs.close();
	buff[length] = '\0';
	file_content = buff;
	delete [] buff;

	return (file_content);
}

void	printErrorPage(std::string error)
{
	std::string error_page = "./html/error_page/" + error + ".html";
	std::string content_str = readFile(error_page);
	std::stringstream ss;
	ss << content_str.size();

	std::cout << "HTTP/1.1 200\n";
	std::cout << "Content-Type: text/html\r\n";
	std::cout << "Content-Length: ";
	std::cout << ss.str();
	std::cout << "\n\n";
	std::cout << content_str;
	std::cout << "\r\n\r\n";
}

int	exec_ls(int *fds, char* const* ls_cmd){
		close(fds[0]);
		dup2(fds[1], 1);
		close(fds[1]);
		return (execve(ls_cmd[0], ls_cmd, NULL) < 0);
}

int get_ls_output(int *fds, std::string & ls_output){
	char	buf[1024];
	int		rd;
	int		status;

	close(fds[1]);
	wait(&status);
	while ((rd = read(fds[0], buf, 1023)) != 0){
		buf[rd] = '\0';
		ls_output += buf;
	}
	close(fds[0]);
	return (status >> 8);
}

int	listDir(std::string & ls_output, const char *path){
	char *ls_cmd[4];
	int pid = 0;
	int	return_code = 0;
	int fds[2];

	ls_cmd[0] = static_cast<char *>(malloc(strlen("/bin/ls") + 1));
	strcpy(ls_cmd[0], "/bin/ls");
	ls_cmd[1] = static_cast<char *>(malloc(strlen("-p") + 1));
	strcpy(ls_cmd[1], "-p");
	ls_cmd[2] = static_cast<char *>(malloc(strlen(path) + 1));
	strcpy(ls_cmd[2], path);
	ls_cmd[3] = NULL;
	if (pipe(fds) < 0)
		return (1);
	pid = fork();
	if (pid == 0) {
		if (exec_ls(fds, ls_cmd) < 0)
			std::cout << "**error execve !!!**\n";
	}
	else if (pid)
		return_code = get_ls_output(fds, ls_output);
	free(ls_cmd[0]);
	free(ls_cmd[1]);
	free(ls_cmd[2]);
	return (return_code);
}

int	find_path(std::string & path, char **env){
	int i = 0;
	size_t equal;
	size_t pathlen;

	for (; *(env + i) && strncmp(*(env + i), "PATH_TRANSLATED=", strlen("PATH_TRANSLATED=")); i++);
	if ( !*(env + i) ){
		return (0);
	}
	path = *(env + i);
	equal = path.find("=", 0);
	pathlen = path.size();
	path = path.substr(equal + 1, pathlen - (equal + 1));
	return (1);
}

std::string ft_getenv(char **envp, const char *str) {
	for (int i = 0; envp[i]; i++) {
		std::string env = std::string(envp[i]);
		std::string key = env.substr(0, env.find('='));
		std::string value = env.substr(env.find('=') + 1);
		if (!key.compare(str))
			return (value);
	}
	return (NULL);
}

std::string auto_index(char **env){
	std::string path;
	std::string ls_output;
	std::vector<std::string>	tab_output;
	std::string					content_body;
	if (!find_path(path, env))
		return (0);

	if (!listDir(ls_output, path.c_str()))
	{
		tab_output.push_back(ls_output.substr(0, (ls_output.find("\n", 0) - (0))));
		for (size_t pos = 0; (pos = ls_output.find("\n", pos)) != std::string::npos; ){
			pos++;
			tab_output.push_back(ls_output.substr(pos, (ls_output.find("\n", pos) - (pos))));
		}
		tab_output.pop_back();

		/*============================*/
		/*             body           */
		/*============================*/

		content_body = "<!DOCTYPE html>";
		content_body += "<html>";
		content_body += "	<head>";
		content_body += "		<meta charset=\"utf-8\">";
		content_body += "		<title>autoindex.html</title>";
		content_body += "		<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css\">";
		content_body += "		<style>";
		content_body += "			@import url('https://fonts.googleapis.com/css2?family=Caveat&display=swap');";
		content_body += "			* {";
		content_body += "				margin: 0;";
		content_body += "				padding: 0;";
		content_body += "				box-sizing: border-box;";
		content_body += "				font-family: 'Caveat', sans-serif;";
		content_body += "				font-size: 50px;";
		content_body += "			}";
		content_body += "			body {";
		content_body += "				display: flex;";
		content_body += "				justify-content: center;";
		content_body += "				align-items: center;";
		content_body += "				min-height: 100vh;";
		content_body += "				background: url(\"https://wallpaperaccess.com/full/173243.jpg\");";
		content_body += "			}";
		content_body += "			.box {";
		content_body += "				width: 80%;";
		content_body += "				text-align: center;";
		content_body += "				display: block;";
		content_body += "				border-bottom: 20px solid rgb(154, 221, 235);";
		content_body += "				border-bottom-left-radius: 10px;";
		content_body += "				border-bottom-right-radius: 10px;";
		content_body += "				border-top-left-radius: 10px;";
		content_body += "				border-top-right-radius: 10px;";
		content_body += "				background: linear-gradient(180deg, rgb(218, 159, 120) 50%, rgb(154, 221, 235) 100%);";
content_body += "";
		content_body += "			}";
		content_body += "			.box h2 {";
		content_body += "				color: #F8F7F3;";
		content_body += "				background: rgb(218, 159, 120);";
		content_body += "				padding: 10px 20px;";
		content_body += "				font-size: 70px;";
		content_body += "				font-weight: 700;";
		content_body += "				border-top-left-radius: 10px;";
		content_body += "				border-top-right-radius: 10px;";
		content_body += "			}";
		content_body += "			li {";
		content_body += "				list-style: none";
		content_body += "			}";
		content_body += "			.box u1 {";
		content_body += "				position: relative;";
		content_body += "				background: #fff;";
		content_body += "			}";
		content_body += "			.box u1 li{";
		content_body += "				list-style: none;";
		content_body += "				padding: 10px;";
		content_body += "				width: 100%;";
		content_body += "				background: #fff;";
		content_body += "				box-shadow: 0 5px 25px rgba(0, 0, 0, .1);";
		content_body += "				transition: transform 0.5s;";
		content_body += "			}";
content_body += "";
		content_body += "			.box ul li:hover {";
		content_body += "				transform: scale(1.1);";
		content_body += "				z-index: 100;";
		content_body += "				background: #25bcff;";
		content_body += "				box-shadow: 0 5px 25px rgba(0,0,0,.2);";
		content_body += "				color: #fff;";
		content_body += "			}";
		content_body += "			.box ul li span img {";
		content_body += "				width: 50px;";
		content_body += "				height: auto;";
		content_body += "				justify-content: center;";
		content_body += "				text-align: center;";
		content_body += "				line-height: 20px;";
		content_body += "				margin: auto;";
		content_body += "			}";
		content_body += "		</style>";
		content_body += "	</head>";
		content_body += "	<body>";
		content_body += "		<div class=\"box\">";
		content_body += "			<h2>Auto Index</h2>";
		content_body += "			<ul>";


		for (std::vector<std::string>::iterator it = tab_output.begin(); it != tab_output.end(); it++)
		{
			std::string slash = "/";
			std::string url = "<a href=\"http://" + ft_getenv(env, "REMOTE_HOST");
			if (ft_getenv(env, "PATH_INFO").substr(6).find(".") == std::string::npos)
			{
				url += ft_getenv(env, "PATH_INFO").substr(6);
				if (ft_getenv(env, "PATH_INFO").substr(6).find_last_of("/") != ft_getenv(env, "PATH_INFO").substr(6).size() - 1)
					url += "/";
				url += *it;
			}
			else
				url += std::string(*it).substr(6);
			std::string img = "https://img.icons8.com/dusk/2x/000000/file--v2.png";
			if (std::string(*it).find("/") != std::string::npos)
				img = "https://i.pinimg.com/originals/67/75/9e/67759e3a6a1dd544deffe5673d021174.png";
			url += "\"><li><span><img src=\"" + img + "\"></span> " + *it + "</li></a>";
			content_body += url;
		}
		content_body += "			</ul>";
		content_body += "		</div>";
		content_body += "	</body>";
		content_body += "</html>";




	}
	else
		printErrorPage("400");
	return (content_body);
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
		str = (*it).first + "=" + map[(*it).first];
		if (!(strs[++i] = (char *)malloc(sizeof(char) * (str.length() + 1))))
			return (NULL);
		strs[i] = ft_strcpy(strs[i], str.c_str());
	}
	strs[++i] = NULL;
	return (strs);
}

#include <sys/types.h>
#include <dirent.h>

bool isADir(const std::string & url) {
    DIR *d;
    d = opendir(url.c_str());
    if (d == NULL)
        return 0;
    closedir(d);
    return 1;
}

std::string Response::autoindex_nocgi() {
	return ("bob");
}

void				Response::buildingResponse(void) {

	if (code == 200)
		checkingMethod();
	
    if (code == 200 && (method == "GET" || method == "DELETE"))
	    getFile();

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

	std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
	if (fileName.find_last_of(".") != std::string::npos && mime.isCgi(extension))
	{
		

		Cgi bob(*this);
		std::map<std::string, std::string> map = bob.create_env();
		char **envp = mtoss(map);
		if (!envp)
			throw std::runtime_error("1");
		std::string path = root + fileName;
		bob.executeCGI(path, envp);
		response += bob.get_final();
		response += "\r\n";
	}
	//redirect
	else if (method == "GET" || (code != 200 && (method == "DELETE" || method == "POST"))) {
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

void		Response::redirectionIndex(void) {
	if (url[url.length() - 1] == '/')
		url = url + server.getIndex(url);
	else
		url = url + server.getIndex(url+"/");
}

void		Response::getFile(void) {
	if (isADir(server.getRoot() + url))
	{

		if (server.getAutoIndex() == "on") {
			isAutoIndex = true;
			fileName = server.getRoot()+ url;
			Cgi bob(*this);
			std::map<std::string, std::string> map = bob.create_env();
			char **envp = mtoss(map);
			if (!envp)
				throw std::runtime_error("1");
			std::string htmlAutoIndex = auto_index(envp);
			this->file = std::vector<char>(htmlAutoIndex.begin(), htmlAutoIndex.end());
//			for(std::vector<char>::iterator it = file.begin(); it != file.end(); ++it)
//				std::cout << CYAN << (*it);
			return ;
		}
		redirectionIndex();
	}
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
        return ;
    }
	this->file = std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
	file.close();
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
	if (isAutoIndex == true) {
		line += "text/html; charset=utf-8\r\n";
		return line;
	}
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
