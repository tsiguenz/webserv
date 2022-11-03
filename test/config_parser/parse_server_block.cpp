#include "test.h"
#include "ConfigParser.hpp"

void	parse_server_block() {
	std::cout << "---------- parse_server_block() ----------\n";
	{
		int	ctn = 0;
		std::string	str;
		str += "	server {\n";
		str += "	server_name webserv.com www.webserv.com\n";
		str += "	listen 127.0.0.1:1024\n";
		str += "	error_page 404 404.html\n";
		str += "	client_max_body 1024\n";
		str += "	autoindex off\n";
		str += "	root /var/www\n";
		str += "	index index.html\n";
		str += "	location /html {\n";
		str += "		methods GET\n";
		str += "		return 301 /html/index.html\n";
		str += "		root /tmp/www\n";
		str += "		autoindex on\n";
		str += "		index html/index.html\n";
		str += "		cgi_allowed_ext .php\n";
		str += "		upload_path /download\n";
		str += "		error_page 404 html/error/404.html\n";
		str += "	}\n";
		str += "}\n";
		ConfigParser	cp;
		VirtualServer	vs = cp._parseServerBlock(str);
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
		Location	l = vs.getLocationList().front();
		if (l.isAllowedMethod("GET") == false)
			ctn++;
		if (l.isAllowedMethod("POST") == true)
			ctn++;
		if (l.isAllowedMethod("DELETE") == true)
			ctn++;
		if (l.getReturnCode() != 301)
			ctn++;
		if (l.getReturnPath() != "/html/index.html")
			ctn++;
		if (l.getRoot() != "/tmp/www")
			ctn++;
		if (l.getAutoIndex() != true)
			ctn++;
		if (l.getIndex() != "html/index.html")
			ctn++;
		if (l.isAllowedExtCgi(".php") != true)
			ctn++;
		if (l.getUploadPath() != "/download")
			ctn++;
		if (l.getErrorPageByCode(404) != "html/error/404.html")
			ctn++;
		assertEq("basic server block", ctn, 0);
	}
	// do the same test with server directive after location
}
