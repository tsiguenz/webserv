#include "Cgi.hpp"

/*
** --------------------------------- METHODS ----------------------------------
*/


// int Cgi::read_output(int fd) {
// 	char buffer[BUFFER_SIZE];
// 	int ret;

// 	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0) {
// 		_output.append(buffer, ret);
// 	}
// 	if (ret == -1)
// 		return (-1);
// 	return (0);
// }

// void	Cgi::manage_output (Response * res) {
// 	(void)res;
// 	inline std::size_t ret;
// 	if ((ret = _output.find("\r\n\r\n")) != std::string::npos) {
// 		_output.erase(0, ret + 4);
// 	}
// 	// res->set_body(_output); //in response : _output is the body to send
// 	close(_fd);
// }


void	Cgi::free_env (void) {
	for (char **it = _env; *it; it++) {
		free(*it);
	}
	free(_env);
}


// void	Cgi::init (void) { 
// 	std::map<std::string, std::string> env_vars = create_env();

// 	if (!(_env = static_cast<char **>(malloc(sizeof(char *) * (env_vars.size() + 1)))))
// 		return ;
// 	for (inline std::size_t i = 0; i < env_vars.size(); i++) {
// 		if ((_env[i] = strdup(env_vars[i].c_str())) == NULL) {
// 			for (inline std::size_t j = 0; j < i; j++)
// 				free(_env[j]);
// 			free(_env);
// 			return ;
// 		}
// 	}
// 	_env[env_vars.size()] = NULL;
// }


std::string Cgi::get_length(void) {
	std::ostringstream s;
	s << std::distance(_response->file.begin(), _response->file.end());
	return (std::string(s.str()));
}

std::string Cgi::get_code(void) {
	std::ostringstream s;
	s << _response->code;
	return (std::string(s.str()));
}

std::map<std::string, std::string>	Cgi::create_env (void) {
	std::map<std::string, std::string> envs;


	envs["SERVER_SOFTWARE"] = "Webserv";
	// std::cout << "SERVER_SOFTWARE" " : " << envs["SERVER_SOFTWARE"] << std::endl;
	envs["SERVER_NAME"] = _response->serverName;
	// // std::cout << "SERVER_NAME" " : " << envs["SERVER_NAME"] << std::endl;
	envs["GATEWAY_INTERFACE"] = "CGI/1.1";
	// // std::cout << "GATEWAY_INTERFACE" " : " << envs["GATEWAY_INTERFACE"] << std::endl;
	envs["SERVER_PROTOCOL"] =  "HTTP/1.1";
	// // std::cout << "SERVER_PROTOCOL" " : " << envs["SERVER_PROTOCOL"] << std::endl;
	envs["SERVER_PORT"] = _response->port;
	// // std::cout << "SERVER_PORT" " : " << envs["SERVER_PORT"] << std::endl;
	envs["REQUEST_METHOD"] = _response->method;
	// // std::cout << "REQUEST_METHOD" " : " << envs["REQUEST_METHOD"] << std::endl;
	envs["REDIRECT_STATUS"] = get_code();
	// // std::cout << "REDIRECT_STATUS" " : " << envs["REDIRECT_STATUS"] << std::endl;
	envs["PATH_INFO"] = std::string(".") + _response->url;//./app/demo/galery.php 
	// // std::cout << "PATH_INFO" " : " << envs["PATH_INFO"] << std::endl;
	envs["PATH_TRANSLATED"] = std::string(".") + _response->url;//./app/demo/galery.php
	// // std::cout << "PATH_TRANSLATED" " : " << envs["PATH_TRANSLATED"] << std::endl;
	envs["SCRIPT_NAME"] = std::string(".") + _response->url;//./app/demo/galery.php
	// // std::cout << "SCRIPT_NAME" " : " << envs["SCRIPT_NAME"] << std::endl;
	envs["SCRIPT_FILENAME"] = std::string(".") + _response->url;//./app/demo/galery.php
	// // std::cout << "SCRIPT_FILENAME" " : " << envs["SCRIPT_FILENAME"] << std::endl;
	envs["QUERY_STRING"] = "";//
	// // std::cout << "QUERY_STRING" " : " << envs["QUERY_STRING"] << std::endl;
	envs["REMOTE_HOST"] = _response->fieldLines["Host"]; // REMOTE_HOST=207.0.0.1
	// // std::cout << "REMOTE_HOST" " : " << envs["REMOTE_HOST"] << std::endl;
	envs["REMOTE_ADDR"] = "";//
	// // std::cout << "REMOTE_ADDR" " : " << envs["REMOTE_ADDR"] << std::endl;
	envs["AUTH_TYPE"] = "";//
	// // std::cout << "AUTH_TYPE" " : " << envs["AUTH_TYPE"] << std::endl;
	envs["REMOTE_USER"] = "";//
	// // std::cout << "REMOTE_USER" " : " << envs["REMOTE_USER"] << std::endl;
	

	// // std::cout << _response->fileName << std::endl;
	// // std::cout << Mediatype.getMediaType(_response->fileName) << std::endl;

	envs["CONTENT_TYPE"] = Mediatype.getMediaType(_response->fileName);//application/x-www-form-urlencoded
	// // std::cout << "CONTENT_TYPE" " : " << envs["CONTENT_TYPE"] << std::endl;
	envs["CONTENT_LENGTH"] = get_length();
	// // std::cout << "CONTENT_LENGTH" " : " << envs["CONTENT_LENGTH"] << std::endl;
	envs["HTTP_ACCEPT"] = _response->fieldLines["Accept"]; //*/*
	// // std::cout << "HTTP_ACCEPT" " : " << envs["HTTP_ACCEPT"] << std::endl;
	envs["HTTP_ACCEPT_LANGUAGE"] = _response->fieldLines["Accept-Language"];
	// // std::cout << "HTTP_ACCEPT_LANGUAGE" " : " << envs["HTTP_ACCEPT_LANGUAGE"] << std::endl;
	envs["HTTP_USER_AGENT"] = _response->httpVersion;
	// // std::cout << "HTTP_USER_AGENT" " : " << envs["HTTP_USER_AGENT"] << std::endl;
	envs["HTTP_COOKIE"] = _response->fieldLines["Cookie"];//PHPSESSID=rbp9ts7miftn1h5jmja928v58u,
	// // std::cout << "HTTP_COOKIE" " : " << envs["HTTP_COOKIE"] << std::endl;
	envs["HTTP_REFERER"] = _response->fieldLines["Referer"];//
	// // std::cout << "HTTP_REFERER" " : " << envs["HTTP_REFERER"] << std::endl;

	return (envs);

	// envs["CONTENT_LENGTH"] = ITOA(_request->get_params("Content-Length"));
	// envs["CONTENT_TYPE"] = _request->get_params("Content-Type");
	// envs["GATEWAY_INTERFACE"] = "CGI/1.1"; // CGI/1.1
	// envs["PATH_INFO"] = _request->get_path(); // path to the file
	// envs["PATH_TRANSLATED"] = _request->get_path(); // path to the file
	// envs["QUERY_STRING"] = _request->get_query_string(); // ?arg1=value1&arg2=value2
	// envs["REDIRECT_STATUS"] = ITOA(_request->get_error_value());
	// envs["REQUEST_METHOD"] = _request->method;
	// envs["SCRIPT_FILENAME"] = _request->get_construct_path(); // /home/../www/index.html
	// envs["SERVER_PROTOCOL"] = "HTTP/1.1"; // usually HTTP/1.1
	// envs["SERVER_PORT"] = ITOA(_request->get_params("Host")); // 8080 for exemple
	// envs["AUTH_TYPE"] = "Basic";
	// envs["SERVER_NAME"] = _request->get_server_name();
	// envs["SERVER_SOFTWARE"] = "webserv/beta";
	// envs["HTTP_ACCEPT"] = _request->get_params("Accept"); // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
	// envs["HTTP_COOKIE"] = _request->get_params("Cookie");
	// envs["HTTP_ACCEPT_LANGUAGE"] = _request->get_params("Accept-Language"); // Accept-Language: en-US,en;q=0.5
	// envs["HTTP_REFERER"] = _request->get_params("Referer");
	// envs["HTTP_USER_AGENT"] = _request->get_params("User-Agent"); // Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0
	// envs["HTTP_ACCEPT_ENCODING"] = _request->get_params("Accept-Encoding"); // Accept-Encoding: gzip, deflate
	// envs["HTTP_ACCEPT_CHARSET"] = _request->get_params("Accept-Charsets"); // Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
	// envs["DOCUMENT_ROOT"] = _request->get_root(); // /home/../www
	// envs["REQUEST_URI"] = _request->get_path(); // /index.html?arg1=value1&arg2=value2
	// envs["UPLOAD_PATH"] = _request->get_upload_path(); // /home/../www/upload
}

std::string Cgi::getProgName(std::string &path)
{
	std::string extension = path.substr(path.find_last_of(".") + 1);
	return (Mediatype.getProgName(extension));
	throw std::runtime_error("501"); //throw
}

void Cgi::executeCGI(std::string &path, char **envp)
{
	int pid, stat, fd[2];

	if (access(path.c_str(), X_OK) == -1)
		throw std::runtime_error("403"); //throw
	if (pipe(fd) != 0)
		throw std::runtime_error("503"); //throw
	pid = fork();
	if (pid == -1)
		throw std::runtime_error("500"); //throw
	if (pid == 0) // child process
	{
		close(fd[0]);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
	
		std::vector<char> copy(_response->body);
		copy.push_back('\0');
		std::FILE* tmpf = std::tmpfile();
		std::fputs(&copy[0], tmpf);
		std::rewind(tmpf);
		if (dup2(fileno(tmpf), STDIN_FILENO) == -1)
			throw std::runtime_error("1");


		if (dup2(fd[1], STDOUT_FILENO) == -1)
			throw std::runtime_error("1");
		close(fd[1]);
		char* binPath = strdup(getProgName(path).c_str());
		char* progPath = strdup(path.c_str());
		char* argv[3] = { binPath, progPath, NULL };
		execve(binPath, argv, envp);
		for (int i = 0 ; envp[i] != NULL ; i++)
			free(envp[i]);
		free(envp);
		free(binPath);
		free(progPath);
		throw std::runtime_error("1");
	}
	else // parent process
	{
		for (int i = 0 ; envp[i] != NULL ; i++)
			free(envp[i]);
		free(envp);
		waitpid(pid, &stat, 0);
		stat = WEXITSTATUS(stat);
		if (stat != 0)
			throw std::runtime_error("500");  //throw
		close(fd[1]);
		int	 ret = 0;
		std::vector<char> buff(1024, 0);
		while ((ret = read(fd[0], &buff[0], 1024)) > 0)
			final_body.insert(final_body.end(), buff.begin(), buff.begin() + ret);
		if (ret < 0)
			throw std::runtime_error("500"); //throw
		close(fd[0]);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/



/* ************************************************************************** */