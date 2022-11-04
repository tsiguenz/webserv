#include "test.h"
#include "ConfigParser.hpp"

void	parse_file_content() {
	std::cout << "---------- parse_file_content() ----------\n";
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	server_name webserv.com www.webserv.com\n";
		str += "	listen 127.0.0.1:1024\n";
		str += "	client_max_body 1024\n";
		str += "	error_page 404 404.html\n";
		str += "	index index.html\n";
		str += "	root /var/www\n";
		str += "	autoindex off\n";
		str += "	return 301 /html/test.html\n";
		str += "	cgi_allowed_ext .py\n";
		str += "	upload_path /tmp/download\n";
		str += "	methods GET POST\n";
		str += "	location /html {\n";
		str += "		error_page 404 html/error/404.html\n";
		str += "		index html/index.html\n";
		str += "		root /tmp/www\n";
		str += "		autoindex on\n";
		str += "		return 301 /html/index.html\n";
		str += "		cgi_allowed_ext .php\n";
		str += "		upload_path /download\n";
		str += "		methods GET DELETE\n";
		str += "	}\n";
		str += "}\n";
		str += "\n";
		str += "\n";
		str += "\n";
		str += "\n";
		str += "server {\n";
		str += "	server_name		   test.html 		\n";
		str += "	listen 192.168.0.1:8080\n";
		str += "	client_max_body 12345\n";
		str += "	error_page 512 512.html\n";
		str += "	index default/index.html\n";
		str += "\n";
		str += "	root /tmp/www\n";
		str += "	autoindex on\n";
		str += "	return 301 301.html\n";
		str += "	cgi_allowed_ext .php\n";
		str += "	upload_path download\n";
		str += "\n";
		str += "	methods GET DELETE\n";
		str += "	location /var/www/html {\n";
		str += "	}\n";
		str += "\n";
		str += "	location /index {\n";
		str += "		error_page 404 404.html\n";
		str += "		index html/index.html\n";
		str += "		root /html\n";
		str += "\n";
		str += "		autoindex off\n";
		str += "		return 306 index.html\n";
		str += "		cgi_allowed_ext .py\n";
		str += "		upload_path /dw\n";
		str += "	}\n";
		str += "}\n";
		ConfigParser	cp;
		cp._parseFileContent(str);
		std::list<VirtualServer>	l = cp.getVirtualServerList();
		std::list<VirtualServer>::const_iterator	it = l.begin();
		VirtualServer	vs = *it;
		if (vs.getServerNames().front() != "webserv.com")
			ctn++;
		if (vs.getServerNames().back() != "www.webserv.com")
			ctn++;
		if (vs.getIp() != "127.0.0.1")
			ctn++;
		if (vs.getPort() != 1024)
			ctn++;
		if (vs.getErrorPageByCode(404) != "404.html")
			ctn++;
		if (vs.getClientMaxBodySize() != 1024)
			ctn++;
		if (vs.getAutoIndex() != false)
			ctn++;
		if (vs.getRoot() != "/var/www")
			ctn++;
		if (vs.getIndex() != "index.html")
			ctn++;
		if (vs.getReturnCode() != 301)
			ctn++;
		if (vs.getReturnPath() != "/html/test.html")
			ctn++;
		if (vs.isAllowedExtCgi(".py") != true)
			ctn++;
		if (vs.getUploadPath() != "/tmp/download")
			ctn++;
		if (vs.isAllowedMethod("GET") != true)
			ctn++;
		if (vs.isAllowedMethod("POST") != true)
			ctn++;
		if (vs.isAllowedMethod("DELETE") != false)
			ctn++;
		Location	loc = vs.getLocationList().front();
		if (loc.getPath() != "/html")
			ctn++;
		if (loc.isAllowedMethod("GET") != true)
			ctn++;
		if (loc.isAllowedMethod("POST") != false)
			ctn++;
		if (loc.isAllowedMethod("DELETE") != true)
			ctn++;
		if (loc.getReturnCode() != 301)
			ctn++;
		if (loc.getReturnPath() != "/html/index.html")
			ctn++;
		if (loc.getRoot() != "/tmp/www")
			ctn++;
		if (loc.getAutoIndex() != true)
			ctn++;
		if (loc.getIndex() != "html/index.html")
			ctn++;
		if (loc.isAllowedExtCgi(".php") != true)
			ctn++;
		if (loc.isAllowedExtCgi(".py") != false)
			ctn++;
		if (loc.getUploadPath() != "/download")
			ctn++;
		if (loc.getErrorPageByCode(404) != "html/error/404.html")
			ctn++;
		assertEq("file with multiple servers (first server)", ctn, 0);
		it++;
		vs = *it;
		if (vs.getServerNames().front() != "test.html")
			ctn++;
		if (vs.getIp() != "192.168.0.1")
			ctn++;
		if (vs.getPort() != 8080)
			ctn++;
		if (vs.getErrorPageByCode(512) != "512.html")
			ctn++;
		if (vs.getClientMaxBodySize() != 12345)
			ctn++;
		if (vs.getAutoIndex() != true)
			ctn++;
		if (vs.getRoot() != "/tmp/www")
			ctn++;
		if (vs.getIndex() != "default/index.html")
			ctn++;
		if (vs.getReturnCode() != 301)
			ctn++;
		if (vs.getReturnPath() != "301.html")
			ctn++;
		if (vs.isAllowedExtCgi(".php") != true)
			ctn++;
		if (vs.getUploadPath() != "download")
			ctn++;
		if (vs.isAllowedMethod("GET") != true)
			ctn++;
		if (vs.isAllowedMethod("POST") != false)
			ctn++;
		if (vs.isAllowedMethod("DELETE") != true)
			ctn++;
//		loc = vs.getLocationList().front();
//		if (loc.isAllowedMethod("GET") != true)
//			ctn++;
//		if (loc.isAllowedMethod("POST") != false)
//			ctn++;
//		if (loc.isAllowedMethod("DELETE") != true)
//			ctn++;
//		if (loc.getReturnCode() != 301)
//			ctn++;
//		if (loc.getReturnPath() != "/html/index.html")
//			ctn++;
//		if (loc.getRoot() != "/tmp/www")
//			ctn++;
//		if (loc.getAutoIndex() != true)
//			ctn++;
//		if (loc.getIndex() != "html/index.html")
//			ctn++;
//		if (loc.isAllowedExtCgi(".php") != true)
//			ctn++;
//		if (loc.isAllowedExtCgi(".py") != false)
//			ctn++;
//		if (loc.getUploadPath() != "/download")
//			ctn++;
//		if (loc.getErrorPageByCode(404) != "html/error/404.html")
//			ctn++;
		assertEq("file with multiple servers (second server)", ctn, 0);
	}
}
