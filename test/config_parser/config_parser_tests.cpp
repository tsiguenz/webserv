#include "test.h"
#include "ConfigParser.hpp"

void	config_parser_tests() {
	std::cout << "---------- Config Parser Tests ----------\n";
	object_managment();
	check_file_name();
	read_file();
	get_block();
	parse_directive();
	parse_location_block();
//	parse_server_block();
//	parse_file_content();
}
