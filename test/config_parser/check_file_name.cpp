#include "test.h"
#include "ConfigParser.hpp"

void	check_file_name() {
	std::cout << "---------- check_file_name() ----------\n";
	ConfigParser	cp;
	// bad extention
	{
		int	ctn = 0;
		try { cp._checkFileName("test/config/bad_extention"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		try { cp._checkFileName("test/config/bad_extention.coonf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file with bad extention", ctn, 2);
	}
	// file name too short
	{
		int	ctn = 0;
		try { cp._checkFileName("conf"); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file name too short", ctn, 1);
	}
}
