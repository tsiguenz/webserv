#include "Cgi.hpp"

/*
** --------------------------------- METHODS ----------------------------------
*/


void	free_env (char **envp) {
	for (int i = 0; envp[i]; i++) {
		free(envp[i]);
	}
	free(envp);
}

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
}

int Cgi::executeCGI(std::string &path, char **envp)
{
	int pid, stat, fd[2];

	if (pipe(fd) != 0) {
		free_env(envp);
		return (1);
	}
	pid = fork();
	if (pid == -1) {
		free_env(envp);
		return (1);
	}
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
			return (free_env(envp), exit(1), 1);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (free_env(envp), exit(1), 1);
		close(fd[1]);
		std::string tmp = getProgName(path);
		const char* binPath = tmp.c_str();
		const char* progPath = path.c_str();
		char* argv[3] = { (char *)binPath, (char *)progPath, NULL };
		execve(binPath, argv, envp);
		free_env(envp);
		exit(1);
	}
	else // parent process
	{
		free_env(envp);
		waitpid(pid, &stat, 0);
		stat = WEXITSTATUS(stat);
		if (stat != 0)
			return (1);
		close(fd[1]);
		int	 ret = 0;
		std::vector<unsigned char> buff(1024, 0);
		while ((ret = read(fd[0], &buff[0], 1024)) > 0)
			final_body.insert(final_body.end(), buff.begin(), buff.begin() + ret);
		if (ret < 0)
			return (1);
		close(fd[0]);
	}
	return (0);
}
