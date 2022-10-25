#include "test.h"
#include "ConfigParser.hpp"

void	parse_file() {
	std::cout << "---------- parse_file() ----------\n";
	int	ctn = 0;
	ConfigParser	cp;
	// file does not exist
	{
		try { cp._parseFile("test/config/does_not_exist.conf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file does not exist test", ctn, 1);
	}
	// no permission
	{
		try { cp._parseFile("test/config/cannot_open.conf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file with no permission test", ctn, 2);
	}
	// directory
	{
		try { cp._parseFile("test/config/directory.conf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("read directory test", ctn, 3);
	}
}
