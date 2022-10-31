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
		std::string		_getServerBlock(std::stringstream& sContent);
		void			_parseFileContent(std::string const& fileContent);
		VirtualServer	_parseServerBlock(std::string const& serverBlock);
		void			_parseLocationBlock(std::string const& locationBlock);
		void			_parseDirective(std::string const& line, VirtualServer& vs);
		void			_parseDirective(std::string const& line, Location& location);

		// VirtualServer
		void			_parseServerNames(std::string const& line, VirtualServer& vs);
		void			_parseListen(std::string const& line, VirtualServer& vs);
		void			_parseErrorPage(std::string const& line, VirtualServer& vs);
		void			_parseClientMaxBodySize(std::string const& line, VirtualServer& vs);
		void			_parseAutoIndex(std::string const& line, VirtualServer& vs);
		void			_parseRoot(std::string const& line, VirtualServer& vs);
		void			_parseIndex(std::string const& line, VirtualServer& vs);
		void			_parseMethods(std::string const& line, VirtualServer& vs);
		void			_parseReturn(std::string const& line, VirtualServer& vs);
		void			_parseCgiAllowed(std::string const& line, VirtualServer& vs);
		void			_parseUploadPath(std::string const& line, VirtualServer& vs);

		// Helper
		std::vector<std::string>	_splitInVector(std::string const& line) const;
};

#endif // CONFIG_PARSER_HPP
