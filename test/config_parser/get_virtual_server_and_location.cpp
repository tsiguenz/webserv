#include "test.h"
#include "ConfigParser.hpp"

void	get_virtual_server_and_location() {
	std::cout << "---------- get_virtual_server_and_location() ----------\n";
	// test getLocationByIndex
	std::string	str;
	str += "server {\n";
	str += "	upload_path /tmp/download\n";
	str += "	location / {\n";
	str += "		upload_path /download\n";
	str += "	}\n";
	str += "	location /html {\n";
	str += "		upload_path /html/download\n";
	str += "	}\n";
	str += "	location /other {\n";
	str += "	}\n";
	str += "}\n";
	ConfigParser	cp;
	cp._parseFileContent(str);
	VirtualServer	vs = cp.getVirtualServerList().front();
//	assertEq("basic location block", vs.getUploadPath("/html/index.html"), "/html/download");
	assertEq("basic location block with subdir path", vs.getUploadPath("/html/hello/index.html"), "/html/download");
//	assertEq("default location block", vs.getUploadPath("/dir/index.html"), "/download");
//	assertEq("default server upload path", vs.getUploadPath("/other/index.html"), "/tmp/download");
}
