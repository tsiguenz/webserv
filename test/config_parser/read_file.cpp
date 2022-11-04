#include "test.h"
#include "ConfigParser.hpp"

void	read_file() {
	std::cout << "---------- read_file() ----------\n";
	// file does not exist
	{
		int	ctn = 0;
		try { ConfigParser	cp("test/config/does_not_exist"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file does not exist test", ctn, 1);
	}
	// no permission
	{
		int	ctn = 0;
		try { ConfigParser	cp("test/config/cannot_open.conf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file with no permission test", ctn, 1);
	}
	// directory
	{
		int	ctn = 0;
		try { ConfigParser	cp("test/config/directory.conf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("read directory test", ctn, 1);
	}
}
