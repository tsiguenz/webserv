#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "VirtualServer.hpp"

class	ConfigParser {
	public:
		// Object managment
		ConfigParser();
		ConfigParser(std::string const& fileName);
		virtual ~ConfigParser();

	private:
		std::string					_fileName;
		std::vector<VirtualServer>	_virtualServerList;

		void	_setDefault();
};

#endif // CONFIG_PARSER_HPP
