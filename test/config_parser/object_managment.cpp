#include "test.h"
#include "ConfigParser.hpp"

void	object_managment() {
	std::cout << "---------- object_managment() ----------\n";
	ConfigParser	cp;
	// default constructor test
	{
		int	ctn = 0;
		VirtualServer	vs = cp.getVirtualServerList().front();
		if (vs.getIp() != "0.0.0.0")
			ctn++;
		if (vs.getPort() != 8080)
			ctn++;
		if (vs.getClientMaxBodySize() != 8192)
			ctn++;
		assertEq("default constructor test", ctn, 0);
	}
}
