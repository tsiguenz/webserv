#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
	std::cout << "default constructor\n";
	_setDefault();
}

ConfigParser::ConfigParser(std::string const& fileName) {
	std::cout << "constructor by filename\n";
	(void) fileName;
}

ConfigParser::~ConfigParser() {

}

void	ConfigParser::_setDefault() {
	std::cout << "setDefault()\n";
}
