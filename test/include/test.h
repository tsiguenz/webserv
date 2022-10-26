#ifndef TEST_H
# define TEST_H

# include "webserv.h"

# define DEFAULT "\e[0m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define BLUE "\e[35m"

template<typename T>
void	assertEq(std::string str, T a, T b) {
	bool	isEq = (a == b);

	if (isEq)
		std::cout << GREEN << "[OK] ";
	else
		std::cout << RED << "[KO] ";
	std::cout << str << " : a = " << a << " b = " << b << DEFAULT << std::endl;

}

void	config_parser_tests();
void	object_managment();
void	check_file_name();
void	read_file();
void	get_block();
void	parse_file_content();
void	parse_server_block();
void	parse_location_block();

#endif // TEST_H
