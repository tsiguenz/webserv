#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "VirtualServer.hpp"
# include <iostream>
# include <fstream> // ifstream
# include <sstream> // stringstream
# include <list>
# include <cstdlib>

class	ConfigParser {
	public:
		// Object managment
		ConfigParser();
		ConfigParser(std::string const& fileName);
		virtual ~ConfigParser();

		// Accessors
		std::list<VirtualServer>	getVirtualServerList() const;
		std::string					getFileContent() const;
		std::string					getFileName() const;
		void						parseFile(std::string const& fileName);

	private:
		std::string					_fileName;
		std::string					_fileContent;
		std::list<VirtualServer>	_virtualServerList;
		std::list<std::string>		_allowedServerDirectives;
		std::list<std::string>		_allowedLocationDirectives;
		std::string const			_WHITESPACES;

#ifdef __IS_TEST__
	public:
#endif

		void			_checkFileName(std::string const& fileName) const;
		void			_initAllowedDirectives();
		bool			_isServerDirective(std::string const& str);
		bool			_isLocationDirective(std::string const& str);
		void			_readFile(std::string const& fileName);
		std::string		_getServerBlock(std::stringstream& sContent) const;
		std::string		_getLocationBlock(std::stringstream& sContent) const;
		void			_parseFileContent(std::string const& fileContent);
		VirtualServer	_parseServerBlock(std::string const& serverBlock);
		Location		_parseLocationBlock(std::string const& locationBlock);
		void			_parseDirective(std::string const& line, VirtualServer& vs);
		void			_parseDirective(std::string const& line, Location& location);

		// Helper
		std::vector<std::string>	_splitInVector(std::string const& line) const;

		// for VirtualServer and Location
		template<typename T>
			void	_parseErrorPage(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() < 3)
					throw std::invalid_argument("error_page directive number of arguments (ex: error_page 400 404 4xx.html)");
				v.erase(v.begin());
				std::string	path = v.back();
				for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end() -1; it++)
					vs.setErrorPage(std::strtod((*it).c_str(), NULL), path);
			}

		template<typename T>
			void	_parseServerNames(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() < 2)
					throw std::invalid_argument("server_name directive bad number of arguments (ex: server_name webserv.com");
				for (std::vector<std::string>::const_iterator it = v.begin() + 1; it != v.end(); it++)
					vs.setServerName(*it);
			}

		template<typename T>
			void	_parseListen(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 2)
					throw std::invalid_argument("listen directive bad number of arguments (ex: listen 127.0.0.1:8080)");
				std::string	str = v.back();
				size_t	sepPos = str.find(":");
				// only port case
				if (sepPos == std::string::npos && str.find(".") == std::string::npos)
					vs.setPort(std::strtod(str.c_str(), NULL));
				// only ip case
				if (sepPos == std::string::npos && str.find(".") != std::string::npos)
					vs.setIp(str);
				// port and ip case
				if (sepPos != std::string::npos) {
					vs.setIp(str.substr(0, sepPos));
					str = str.substr(sepPos + 1);
					vs.setPort(std::strtod(str.c_str(), NULL));
				}
			}

		template<typename T>
			void	_parseClientMaxBodySize(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 2)
					throw std::invalid_argument("client_max_body directive bad syntax");
				int	maxBodySize = strtod(v.back().c_str(), NULL);
				if (maxBodySize <= 0)
					throw std::invalid_argument("client_max_body bad value");
				vs.setClientMaxBodySize(maxBodySize);
			}

		template<typename T>
			void	_parseAutoIndex(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 2)
					throw std::invalid_argument("autoindex directive bad number of arguments (ex: autoindex on)");
				std::string	autoIndex;
				if (v.back() == "on")
					autoIndex = "on";
				else if (v.back() == "off")
					autoIndex = "off";
				else
					throw std::invalid_argument("autoindex directive bad argument (argument must be on or off");
				vs.setAutoIndex(autoIndex);
			}

		template<typename T>
			void	_parseRoot(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 2)
					throw std::invalid_argument("root directive bad number of arguments (ex: root /var/www)");
				vs.setRoot(v.back());
			}

		template<typename T>
			void	_parseIndex(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 2)
					throw std::invalid_argument("index directive number of arguments (ex: index index.html)");
				vs.setIndex(v.back());
			}

		template<typename T>
			void	_parseMethods(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() == 1)
					throw std::invalid_argument("methods directive number of arguments (ex: methods GET POST DELETE");
				for (std::vector<std::string>::const_iterator it = v.begin() + 1; it != v.end(); it++) {
					if (*it == "GET" || *it == "POST" || *it == "DELETE")
						vs.setAllowedMethod(*it);
					else
						throw std::invalid_argument("methods directive bad argument (must be GET or/and POST or/and DELETE");
				}
			}

		template<typename T>
			void	_parseReturn(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 3)
					throw std::invalid_argument("return directive bad number of arguments (ex: return 404 google.com)");
				int	code = std::strtod(v.at(1).c_str(), NULL);
				if (code == 0)
					throw std::invalid_argument("return directive bad code (ex: return 404 google.com)");
				vs.setReturn(code, v.at(2));
			}

		template<typename T>
			void	_parseCgiAllowed(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() == 1)
					throw std::invalid_argument("cgi_allowed_ext directive bad number of arguments (ex: cgi_allowed_ext .py .php)");
				for (std::vector<std::string>::const_iterator it = v.begin() + 1; it != v.end(); it++) {
					if ((*it).at(0) == '.')
						vs.setAllowedExtCgi(*it);
					else
						throw std::invalid_argument("cgi_allowed_ext directive bad argument (must start by .)");
				}
			}

		template<typename T>
			void	_parseUploadPath(std::string const& line, T& vs) {
				std::vector<std::string>	v = _splitInVector(line);
				if (v.size() != 2)
					throw std::invalid_argument("upload_path directive bad number of arguments (ex: upload_path html/download)");
				vs.setUploadPath(v.back());
			}
};

#endif // CONFIG_PARSER_HPP
