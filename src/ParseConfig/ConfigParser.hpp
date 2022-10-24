#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "VirtualServer.hpp"
# include <iostream>
# include <fstream> // ifstream
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

		void	_checkFileName(std::string const& fileName) const;
		void	_readFile();
};

#endif // CONFIG_PARSER_HPP
