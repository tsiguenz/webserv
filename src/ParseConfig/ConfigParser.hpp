#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "VirtualServer.hpp"
# include <iostream>
# include <fstream> // ifstream
# include <sstream> // stringstream
# include <list>

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

// for testing
#ifdef __IS_TEST__
	public:
#else
	private:
#endif
		void	_checkFileName(std::string const& fileName) const;
		void	_parseFile(std::string const& fileName);
		void	_parseServerBlock(std::stringstream& serverBlock);
//		void	_parseLocationBlock();
};

#endif // CONFIG_PARSER_HPP
