#ifndef CGI_HPP
# define CGI_HPP

# include "Response.hpp"
# include <string>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fstream>
#include <unistd.h>

# define BUFFER_SIZE	1025

class Response;

class Cgi
{

	public:

		// Cgi (void) :_env(NULL), _exec(""), _extension(""), _fd(-1), _response(NULL),
		// 					_output(""), Mediatype()
		// {
		// }
		

		Cgi (Response & response) : _env(NULL), _exec(""), _extension(""), _fd(-1), 
							_response(&response), _output(""), Mediatype()
		{
		}

		// Cgi (const std::string & ext, const std::string & path, Response * res) :
		// 	_env(NULL), _exec(path), _extension(ext), _fd(-1), _response(res), _output(""), Mediatype()
		// {
		// }

		// Cgi (const Cgi & copy) {
		// 	*this = copy;
		// }

		~Cgi() {};

		Cgi & operator= (const Cgi & copy) {
			if (this == &copy) {return (*this);}

			_env = copy._env;
			_exec = copy._exec;
			_extension = copy._extension;
			_fd = copy._fd;
			_response = copy._response;
			_output = copy._output;
			return (*this);
		}

		void				set_env (char ** env) 					{ _env = env; }
		void				set_exec (std::string exec) 			{ _exec = exec; }
		void				set_extension (std::string extension) 	{ _extension = extension; }
		void				set_fd (int fd) 						{ _fd = fd; }
		void				set_response (Response * response) 		{ _response = response; }
		void				set_output (std::string output) 		{ _output = output; }

		char ** 			get_env (void) const 					{ return _env; }
		std::string 		get_exec (void) const 					{ return _exec; }
		std::string 		get_extension (void) const 				{ return _extension; }
		int					get_fd (void) const 					{ return _fd; }
		Response *			get_response (void) const 				{ return _response; }
		std::string 		get_output (void) const 				{ return _output; }
		std::string			getProgName(std::string &path);
		void				executeCGI(std::string &path, char **envp);
		std::string			get_length(void);
		std::string			get_code(void);
		
		void				clear (void) {
			_env = NULL;
			_exec = "";
			_extension = "";
			_fd = -1;
			_response = NULL;
			_output = "";
		}

		int		read_output(int fd);
		void	manage_output (Response * res);
		void	free_env (void);
		void	init (void);
		std::map<std::string, std::string>	create_env (void);

	private:
		char ** 	_env;
		std::string _exec;
		std::string _extension;
		int			_fd;
		Response *	_response;
		// localisation
		std::string _output;
		MediaType	Mediatype;
		std::string final_body;
};

#endif /* ************************************************************* CGI_H */