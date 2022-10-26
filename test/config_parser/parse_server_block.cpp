#include "test.h"
#include "ConfigParser.hpp"

void	parse_server_block() {
	std::cout << "---------- parse_server_block() ----------\n";
	int	ctn = 0;
	(void) ctn;
	std::string	str;
	str += "server {\n";
	str += "	server_name webserv.com\n";
	str += "}\n";
	ConfigParser	cp;
	VirtualServer	vs = cp._parseServerBlock(str);
}
