#ifndef TEST_H
# define TEST_H

# include "webserv.h"

template<typename T, typename U>
void	assertEq(std::string str, T a, U b) {
	bool	isEq = (a == b);

	if (isEq)
		std::cout << GREEN << "[OK] " << str << WHITE << std::endl;
	else
		std::cout << RED << "[KO] " << str << " : a = " << a << " b = " << b << WHITE << std::endl;
}

void	config_parser_tests();
void	check_file_name();
void	read_file();
void	get_block();
void	parse_directive();
void	parse_file_content();
void	parse_server_block();
void	parse_location_block();

#endif // TEST_H
