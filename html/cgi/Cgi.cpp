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
	envs["SERVER_NAME"] = _response->serverName;
	envs["GATEWAY_INTERFACE"] = "CGI/1.1";
	envs["SERVER_PROTOCOL"] =  "HTTP/1.1";
	envs["SERVER_PORT"] = _response->port;
	envs["REQUEST_METHOD"] = _response->method;
	envs["REDIRECT_STATUS"] = get_code();
	envs["PATH_INFO"] = std::string("./") + _response->root + _response->url;//./app/demo/galery.php 
	envs["PATH_TRANSLATED"] = std::string("./") + _response->root + _response->url;//./app/demo/galery.php
	envs["SCRIPT_NAME"] = std::string("./") + _response->root + _response->url;//./app/demo/galery.php
	envs["SCRIPT_FILENAME"] = std::string("./") + _response->root + _response->url;//./app/demo/galery.php
	envs["QUERY_STRING"] = _response->queryString;
	envs["REMOTE_HOST"] = _response->fieldLines["Host"]; // REMOTE_HOST=207.0.0.1
	envs["REMOTE_ADDR"] = "";//
	envs["AUTH_TYPE"] = "";//
	envs["REMOTE_USER"] = "";//
	if (Mediatype.isTypeSupported(_response->fileName))
		envs["CONTENT_TYPE"] = Mediatype.getMediaType(_response->fileName);//application/x-www-form-urlencoded
	else
		envs["CONTENT_TYPE"] = "";

	envs["CONTENT_LENGTH"] = get_length();
	envs["HTTP_ACCEPT"] = _response->fieldLines["Accept"]; //*/*
	envs["HTTP_ACCEPT_LANGUAGE"] = _response->fieldLines["Accept-Language"];
	envs["HTTP_USER_AGENT"] = _response->httpVersion;
	envs["HTTP_COOKIE"] = _response->fieldLines["Cookie"];//PHPSESSID=rbp9ts7miftn1h5jmja928v58u,
	envs["HTTP_REFERER"] = _response->fieldLines["Referer"];//
	return (envs);
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
	
		std::vector<unsigned char> copy(_response->body);
		copy.push_back('\0');
		std::FILE* tmpf = std::tmpfile();
		std::fputs((char*) &copy[0], tmpf);
		std::rewind(tmpf);
		if (dup2(fileno(tmpf), STDIN_FILENO) == -1)
			exit(1);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(1);
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
		exit(1);
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
		std::vector<unsigned char> buff(1024, 0);
		while ((ret = read(fd[0], &buff[0], 1024)) > 0)
			final_body.insert(final_body.end(), buff.begin(), buff.begin() + ret);
		if (ret < 0)
			throw std::runtime_error("500"); //throw
		close(fd[0]);
	}
}
