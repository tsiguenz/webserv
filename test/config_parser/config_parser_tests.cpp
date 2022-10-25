#include "test.h"
#include "ConfigParser.hpp"

void	config_parser_tests() {
	std::cout << "---------- Config Parser Tests ----------\n";
	object_managment();
	check_file_name();
	parse_file();
	parse_server_block();
	parse_location_block();
}
