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
Response::Response( Request  src): mime(), isAutoIndex(false){

	code = src.parsingCode;
	method = src.method;
	url = src.url;
	httpVersion = src.httpVersion;
	fieldLines = src.fieldLines;
	server = src.server;
	body = src.body;
	root = server.getRoot();
	queryString = src.queryString;
	serverName = server.getServerNames().front();
	std::stringstream ss;
	ss << server.getPort();
	port = ss.str();

	buildingResponse();
	// printResponse();
}

Response::Response(Response const& rhs)
{ *this = rhs; }

Response&	Response::operator=(Response const& rhs) {
	method = rhs.method;
	url = rhs.url;
	httpVersion = rhs.httpVersion;
	fieldLines = rhs.fieldLines;
	body = rhs.body;
	root = rhs.root;
	serverName = rhs.serverName;
	queryString = rhs.queryString;
	port = rhs.port;
	fileName = rhs.fileName;
	file = rhs.file;
	response = rhs.response;
	code = rhs.code;
	mime = rhs.mime;
	server = rhs.server;
	statusCodes = rhs.statusCodes;
	return *this;
}

/*
 ** -------------------------------- DESTRUCTOR --------------------------------
 */

Response::~Response() { }


/*
 ** --------------------------------- AUTO INDEX HTML ----------------------------------
 */

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
	if (!ls_cmd[0])
		return (1);
	strcpy(ls_cmd[0], "/bin/ls");
	ls_cmd[1] = static_cast<char *>(malloc(strlen("-p") + 1));
	if (!ls_cmd[1])
		return (free(ls_cmd[0]), 1);
	strcpy(ls_cmd[1], "-p");
	ls_cmd[2] = static_cast<char *>(malloc(strlen(path) + 1));
	if (!ls_cmd[2])
		return (free(ls_cmd[0]), free(ls_cmd[1]), 1);
	strcpy(ls_cmd[2], path);
	ls_cmd[3] = NULL;
	if (pipe(fds) < 0)
		return (free(ls_cmd[0]), free(ls_cmd[1]), free(ls_cmd[2]), 1);
	pid = fork();
	if (pid == 0) {
		if (exec_ls(fds, ls_cmd) < 0)
			std::cout << "**error execve !!!**\n";
		free(ls_cmd[0]);
		free(ls_cmd[1]);
		free(ls_cmd[2]);
		exit(1);
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

std::string	Response::handleErrorCgi() {
	
	response.clear();
	std::string preResponse;
	code = 500;
	std::string errorPage = server.errorHandler.generateErrorHtml(code); 
	file.clear();
	file.insert(file.begin(), errorPage.begin(), errorPage.end());
	fileName = "error.html";
	
	preResponse = getResponse();
	preResponse += getTime();
	preResponse += getServerName();
	preResponse += getConnectionType();
	preResponse += getTypeContent();
	preResponse += getLength();
	preResponse += "\r\n\r\n";
	preResponse += std::string(file.begin(), file.end());
	return (preResponse);
}

std::string Response::auto_index(char **env){
	std::string path;
	std::string ls_output;
	std::vector<std::string>	tab_output;
	std::string					content_body;
	if (!find_path(path, env))
		return (handleErrorCgi());

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
		content_body += "		<title>autoindex</title>";
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
			url += ft_getenv(env, "PATH_INFO").substr(server.getRoot(url).length() + 2);
			if (ft_getenv(env, "PATH_INFO").substr(server.getRoot(url).length() + 2).find_last_of("/") != ft_getenv(env, "PATH_INFO").substr(server.getRoot(url).length() + 2).size() - 1)
				url += "/";
			url += *it;
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
		return (handleErrorCgi()); //error return null puis page handleerror
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
		if (!(strs[++i] = (char *)malloc(sizeof(char) * (str.length() + 1)))) {
			for (;i != -1; i--)
				free(strs[i]);
			free (strs);
			return (NULL);
		}
		strs[i] = ft_strcpy(strs[i], str.c_str());
	}
	strs[++i] = NULL;
	return (strs);
}

void	Response::buildingResponse(void) {
	if (code == 200)
		checkingMethod();
	if (code == 200 && (method == "GET" || method == "DELETE"))
		getFile();
	if (code == 200 && method == "DELETE")
		deleteFile();
	if (code == 200 && method == "POST")
		postFile();
	if (server.getReturnCode(url) == code)
		code = 302;
	if (code > 399)
		handleError();
	response = getResponse();
	redirectionUrl();
	response += getTime();
	response += getServerName();
	response += getConnectionType();
	std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
	if (fileName.find_last_of(".") != std::string::npos && mime.isCgi(extension)
			&& server.isAllowedExtCgi(std::string(".") + extension) == true) {
		Cgi	cgi(*this);
		std::map<std::string, std::string>	map = cgi.create_env();
		char	**envp = mtoss(map);
		if (!envp) {
			response += handleErrorCgi();
			return ;
		}
		if (cgi.executeCGI(fileName, envp) == 1){
			response += handleErrorCgi();
			return ;
		}
		response += cgi.get_final();
		response += "\r\n";
	}
	//redirect
	else if (method == "GET" || code != 200 ) {
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
	if (server.isAllowedMethod(method, (url[url.size() -1] == '/') ? url : url + '/'))
		return ;
	if (method == "DELETE" || method == "GET" || method == "POST"
			|| method == "HEAD" || method == "PATCH" || method == "PUT"
			|| method == "OPTIONS" || method == "CONNECT" || method == "TRACE") {
		code = 405;
		return ;
	}
	code = 400;
	return ;
}

void		Response::redirectionIndex(void) {
		url = url + server.getIndex(url);
}

void		Response::getFile(void) {

	std::string root = server.getRoot(url);
	if (root[0] == '/')
		root.erase(0,1);
	std::string path = root + url;
	if (path[0] == '/' && path.length() > 1)
		path.erase(0,1);
	if (isADir(path))
	{
		if (url[url.length() - 1] != '/')
			url += "/";
		if (server.getAutoIndex(url) == "off")
			redirectionIndex();
		path = root + url;
		if (path[0] == '/')
			path.erase(0,1);
		if (url[url.size() - 1] != '/')
			url.push_back('/');
		
		if (isADir(path) && server.getAutoIndex(url) == "on") {
			isAutoIndex = true;
			fileName = server.getRoot()+ url;
			Cgi	cgi(*this);
			std::map<std::string, std::string> map = cgi.create_env();
			char	**envp = mtoss(map);
			if (!envp) {
				code = 500 ;
				return ;
			}

			std::string htmlAutoIndex = auto_index(envp);
			if (htmlAutoIndex.empty()) {
				code = 500 ;
				return ;
			}
			this->file = std::vector<char>(htmlAutoIndex.begin(), htmlAutoIndex.end());
			for (int i = 0; envp[i]; i++) {
				free(envp[i]);
			}
			free(envp);
			return ;
		}
		if(isADir(path)) {
			code = 404 ;
			return ;
		}
	}
	if (!mime.isExtensionSupported(path) && method == "GET") {	
		code = 415;
		return;
	}
	std::ifstream file(path.c_str());
	if(file.fail() && method == "GET") {
		code = 404;
		return ;
	}
	if (method == "GET")
		this->file = std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
	file.close();
	fileName = path;
	return ;
}

void		Response::redirectionUrl(void) {
	if (code != 302)
		return ;
	response += "Location: ";
	response += server.getReturnPath(url);
	response += "\r\n";
}

/*
 ** --------------------------------- POST METHODE ----------------------------------
 */

void		Response::postFile(void) {
	fileName = server.getRoot() + url;
	if (fieldLines["Content-Type"].find("multipart/form-data") == 0)
		_postFormData();
	if (fieldLines["Content-Type"].find("application/x-www-form-urlencoded") == 0) {
		queryString = std::string(body.begin(), body.end());
	}
}

void	Response::_postFormData() {
	size_t	pos = fieldLines["Content-Type"].find("boundary=");
	if (pos == std::string::npos) { code = 400; return ; }
	for (std::vector<unsigned char> v = _getFormDataBlock(); v.empty() == false; v = _getFormDataBlock()) {
		if (code == 400)
			return ;
		_postFormDataBlock(v);
	}
}

std::vector<unsigned char>	Response::_getFormDataBlock() {
	static size_t	pos = 0;
	std::vector<unsigned char>	ret;
	size_t	posDelim = fieldLines["Content-Type"].find("boundary=");
	if (posDelim == std::string::npos) { code = 400; return ret; }
	std::string	delim = fieldLines["Content-Type"].substr(posDelim + 9);
	size_t	startBlock = _itFind(body.begin(), body.end(), delim, pos);
	size_t	endBlock = _itFind(body.begin(), body.end(), delim, startBlock + delim.size());
	if (startBlock == std::string::npos || endBlock == std::string::npos) {
		pos = 0;
		return ret;
	}
	endBlock += delim.size();
	pos = endBlock;
	for (; startBlock != endBlock; startBlock++)
		ret.push_back(body[startBlock]);
	return ret;
}

// create new file with data block informations
void	Response::_postFormDataBlock(std::vector<unsigned char> const& v) {
	std::string	fileName;
	// get delim
	size_t	posDelim = fieldLines["Content-Type"].find("boundary=");
	if (posDelim == std::string::npos) { code = 400; return ; }
	std::string	delim = fieldLines["Content-Type"].substr(posDelim + 9);
	// get upload path
	std::string	uploadPath = server.getUploadPath(root + url);
	if (*(uploadPath.end() - 1) != '/' && uploadPath.empty() == false)
		uploadPath.push_back('/');
	// get filename
	size_t	pos = _itFind(v.begin(), v.end(), "filename=\""); //prlb la
	if (pos == std::string::npos) { code = 400; return ; }
	pos += 10;
	while (v[pos] != '"')
		fileName.push_back(v[pos++]);
	// get file content
	size_t	posStart = _itFind(v.begin(), v.end(), "\r\n\r\n");
	if (posStart == std::string::npos) { code = 400; return ; }
	posStart += 4;
	size_t	posEnd = _itFind(v.begin() + posStart, v.end(), delim);
	if (posEnd == std::string::npos) { code = 400; return ; }
	// 4 is the lenght of the "\r\n" and "--"
	posEnd += posStart - 4;
	fileName = root + uploadPath + fileName;
	// write in file
	std::ofstream	of(fileName.c_str());
	if (of.good() == false) { code = 400; of.close(); return ; } //prbl la
	for (std::vector<unsigned char>::const_iterator it = v.begin(); posStart != posEnd && of.good() == true; posStart++)
		of.put(it[posStart]);
	of.close();
}

/*
 ** --------------------------------- DELETE METHODE ----------------------------------
 */

void	Response::deleteFile(void) {
	std::string root = server.getRoot();
	if (fileName.find("..") != std::string::npos) {
		code = 400;
		return;
	}
	if (remove((fileName).c_str()) != 0)
		code = 404;
}

/*
 ** --------------------------------- HANDLE ERROR ----------------------------------
 */

void		Response::handleError(void) {

	if(server.getErrorPages(url).count(code)) {

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

void	Response::printResponse(void) const
{ std::cout << response << std::endl; }
