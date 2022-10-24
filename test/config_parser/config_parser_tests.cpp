#include "test.h"

void	config_parser_tests() {
	// default constructor test
	{
		int	ctn = 0;
		ConfigParser	servInfos;
		VirtualServer	vs = servInfos.getVirtualServerList().front();
		if (vs.getIp() != "127.0.0.1")
			ctn++;
		if (vs.getPort() != 8080)
			ctn++;
		if (vs.getErrorPageByCode(404) != "html/404.html")
			ctn++;
		if (vs.getClientMaxBodySize() != -1)
			ctn++;
		assertEq("default constructor test", ctn, 0);
	}
	// file does not exist
	{
		int	ctn = 0;
		try {
			ConfigParser	servInfos("test/config/does_not_exist.conf");
		}
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file does not exist test", ctn, 1);
	}
	// bad extention
	{
		int	ctn = 0;
		try {
			ConfigParser	servInfos("test/config/bad_extention");
		}
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		try {
			ConfigParser	servInfos("test/config/bad_extention.coonf");
		}
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("file with bad extention test", ctn, 1);
	}

	try {
		ConfigParser	servInfos("test/config/one_server_block_without_location.conf");
		std::cout << servInfos.getFileContent();
	}
	catch (std::exception const& e) {
		std::cerr << e.what();
	}
}
