#include "test.h"
#include "ConfigParser.hpp"

void	object_managment() {
	std::cout << "---------- object_managment() ----------\n";
	int	ctn = 0;
	ConfigParser	cp;
	// default constructor test
	{
		VirtualServer	vs = cp.getVirtualServerList().front();
		if (vs.getIp() != "127.0.0.1")
			ctn++;
		if (vs.getPort() != 8080)
			ctn++;
		if (vs.getClientMaxBodySize() != -1)
			ctn++;
		assertEq("default constructor test", ctn, 0);
	}
}
