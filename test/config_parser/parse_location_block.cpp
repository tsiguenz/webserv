#include "test.h"
#include "ConfigParser.hpp"

void	parse_location_block() {
	std::cout << "---------- parse_location_block() ----------\n";
	ConfigParser	cp;
	{
		std::string			locationBlock;
		locationBlock += "location {\n";
		locationBlock += "methods GET\n";
		locationBlock += "}\n";
		cp._parseLocationBlock(locationBlock);
		std::cout << cp.getFileContent();
	}
}
