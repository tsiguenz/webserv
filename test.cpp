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


int main(int ac, char **av, char **env){
	(void) ac;
	(void) av;
	std::string path;
	std::string ls_output;
	std::vector<std::string>	tab_output;
	std::stringstream			ss;
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

		content_body = "<!DOCTYPE html>\n";
		content_body += "<html>\n";
		content_body += "	<head>\n";
		content_body += "		<meta charset=\"utf-8\">\n";
		content_body += "		<title>autoindex.html</title>\n";
		content_body += "		<link rel=\"icon\" href=\"https://cdn.intra.42.fr/users/279a6ebab9ca714cb183c75921ab3332/slahlou.JPG\" />\n";
		content_body += "		<style>\n";
		content_body += "			body\n";
		content_body += "			{\n";
		content_body += "				width: 100%;\n";
		content_body += "				height: 100%;\n";
		content_body += "				background-color: rgba(66, 192, 150, 0.4);\n";
		content_body += "			}\n";
		content_body += "			.container\n";
		content_body += "			{\n";
		content_body += "				display: fex;\n";
		content_body += "				justify-content: center;\n";
		content_body += "				text-align: center;\n";
		content_body += "				margin-left: 30%;\n";
		content_body += "				margin-right: 30%;\n";
		content_body += "				height: 60%;\n";
		content_body += "			}\n";
		content_body += "			.card\n";
		content_body += "			{\n";
		content_body += "				border: rgba(0, 0, 0, 0) solid 2px;\n";
		content_body += "				border-radius: 20px;\n";
		content_body += "				color: rgb(0, 0, 0);\n";
		content_body += "				width: 300px;\n";
		content_body += "				box-shadow: 5px 10px 0px 3px rgb(124, 121, 121);\n";
		content_body += "				margin: 10%;\n";
		content_body += "			}\n";
		content_body += "	</head>\n";
		content_body += "		</style>\n";
		content_body += "	<body>\n";
		content_body += "		<div class=\"container\">\n";
		// content_body += "			<h1 class=\"card\">" + std::string(getenv("PATH_INFO")) + "</h1>\n";
		content_body += "			<ul>\n";

		for (std::vector<std::string>::iterator it = tab_output.begin(); it != tab_output.end(); it++)
		{
			// content_body += "<a href=\"http://" + std::string(getenv("REMOTE_HOST")) + ":" + std::string(getenv("SERVER_PORT")) + std::string(getenv("PATH_INFO")) + *it + "\"><li class=\"card\">" + *it + "</li></a>\n";
			content_body += *it + "  " + *it + "\n";
		}


		content_body += "			</ul>\n";
		content_body += "		</div>\n";
		content_body += "	</body>\n";
		content_body += "</html>\n";

			
		/*==========================*/
		/*    make header whith env */
		/*==========================*/
		ss << content_body.size();

		std::cout << "HTTP/1.1 200 OK\n\n";
		std::cout << "Content-Type: text/html\r\n\n";
		std::cout << "Content-Length: ";
		std::cout <<  ss.str();
		std::cout << "\n\n";

		std::cout << content_body;

		/*============================*/
		/*             footer          */
		/*============================*/
		std::cout << "\r\n\r\n";
	}
	else
		printErrorPage("400");
	return (0);
}