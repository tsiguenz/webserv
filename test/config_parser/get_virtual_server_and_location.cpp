#include "test.h"
#include "ConfigParser.hpp"

void	get_virtual_server_and_location() {
	std::cout << "---------- get_virtual_server_and_location() ----------\n";
	// test getLocationByIndex
	{
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
		assertEq("basic location block upload path", vs.getUploadPath("/html/index.html"), "/html/download");
		assertEq("basic location block with subdir path upload path", vs.getUploadPath("/html/hello/index.html"), "/html/download");
		assertEq("default location block upload path", vs.getUploadPath("/dir/index.html"), "/download");
		assertEq("default server upload path", vs.getUploadPath("/other/index.html"), "/tmp/download");
	}
	{
		std::string	str;
		str += "server {\n";
		str += "	root /d\n";
		str += "	location /a {\n";
		str += "		root /a\n";
		str += "	}\n";
		str += "	location /b {\n";
		str += "		root /b\n";
		str += "	}\n";
		str += "	location /b/b/b {\n";
		str += "		root /b/b/b\n";
		str += "	}\n";
		str += "	location /c {\n";
		str += "	}\n";
		str += "}\n";
		ConfigParser	cp;
		cp._parseFileContent(str);
		VirtualServer	vs = cp.getVirtualServerList().front();
		assertEq("basic location block root", vs.getRoot("/a/a.html"), "/a");
		assertEq("multiple directory root", vs.getRoot("/b/b/b/b.html"), "/b/b/b");
		assertEq("path multiple directory trigger basic location", vs.getRoot("/b/b/b.html"), "/b");
		assertEq("trigger server root as default in location", vs.getRoot("/c.html"), "/d");
		assertEq("trigger server root", vs.getRoot("/c/c.html"), "/d");
	}
	{
		std::string	str;
		str += "server {\n";
		str += "	root /d\n";
		str += "	location /a {\n";
		str += "		root /a\n";
		str += "	}\n";
		str += "	location /ab {\n";
		str += "		root /ab\n";
		str += "	}\n";
		str += "}\n";
		ConfigParser	cp;
		cp._parseFileContent(str);
		VirtualServer	vs = cp.getVirtualServerList().front();
		assertEq("start of directory name match", vs.getRoot("/abc/index.html"), "/d");
	}
}
