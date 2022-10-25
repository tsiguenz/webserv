#include "test.h"
#include "ConfigParser.hpp"

void	check_file_name() {
	std::cout << "---------- check_file_name() ----------\n";
	int	ctn = 0;
	ConfigParser	cp;
	// bad extention
	{
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
		assertEq("file with bad extention test", ctn, 2);
	}
}
