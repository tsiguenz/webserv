#include "test.h"
#include "ConfigParser.hpp"

void	parse_server_block() {
	std::cout << "---------- parse_server_block() ----------\n";
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
		Location	l = vs.getLocationList().front();
		if (l.isAllowedMethod("GET") != true)
			ctn++;
		if (l.isAllowedMethod("POST") != false)
			ctn++;
		if (l.isAllowedMethod("DELETE") != true)
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
		if (l.isAllowedExtCgi(".py") != false)
			ctn++;
		if (l.getUploadPath() != "/download")
			ctn++;
		if (l.getErrorPageByCode(404) != "html/error/404.html")
			ctn++;
		assertEq("basic server block", ctn, 0);
	}
	// do the same test with server directive after location
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	server_name webserv.com www.webserv.com\n";
		str += "	listen 127.0.0.1:1024\n";
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
		str += "	error_page 404 404.html\n";
		str += "	client_max_body 1024\n";
		str += "	autoindex off\n";
		str += "	root /var/www\n";
		str += "	index index.html\n";
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
		assertEq("directive before and after location", ctn, 0);
	}
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	server_name test.com\n";
		str += "	listen 192.168.0.1:8080\n";
		str += "	location /html {\n";
		str += "		return 301 /html/index.html\n";
		str += "		root /tmp/www\n";
		str += "		autoindex on\n";
		str += "		index html/index.html\n";
		str += "		cgi_allowed_ext .php\n";
		str += "		upload_path /download\n";
		str += "	}\n";
		str += "	error_page 404 404.html\n";
		str += "	client_max_body 1024\n";
		str += "	autoindex off\n";
		str += "	root /var/www\n";
		str += "	index index.html\n";
		str += "	methods GET\n";
		str += "}\n";
		ConfigParser	cp;
		VirtualServer	vs = cp._parseServerBlock(str);
		if (vs.getServerNames().front() != "test.com")
			ctn++;
		if (vs.getIp() != "192.168.0.1")
			ctn++;
		if (vs.getPort() != 8080)
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
		if (l.getErrorPageByCode(404) != "404.html")
			ctn++;
		assertEq("default value set in server block but not in location", ctn, 0);
	}
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	server_name test.com\n";
		str += "	listen 127.0.0.1:1024\n";
		str += "	client_max_body 1024\n";
		str += "	error_page 404 404.html\n";
		str += "	index index.html\n";
		str += "	root /var/www\n";
		str += "	autoindex on\n";
		str += "	return 301 /html/test.html\n";
		str += "	cgi_allowed_ext .py\n";
		str += "	upload_path /tmp/download\n";
		str += "	methods GET POST\n";
		str += "	location /html {\n";
		str += "	}\n";
		str += "}\n";
		ConfigParser	cp;
		VirtualServer	vs = cp._parseServerBlock(str);
		if (vs.getServerNames().front() != "test.com")
			ctn++;
		if (vs.getIp() != "127.0.0.1")
			ctn++;
		if (vs.getPort() != 1024)
			ctn++;
		if (vs.getErrorPageByCode(404) != "404.html")
			ctn++;
		if (vs.getClientMaxBodySize() != 1024)
			ctn++;
		Location	l = vs.getLocationList().front();
		if (l.getPath() != "/html")
			ctn++;
		if (l.isAllowedMethod("GET") != true)
			ctn++;
		if (l.isAllowedMethod("POST") != true)
			ctn++;
		if (l.isAllowedMethod("DELETE") != false)
			ctn++;
		if (l.getReturnCode() != 301)
			ctn++;
		if (l.getReturnPath() != "/html/test.html")
			ctn++;
		if (l.getRoot() != "/var/www")
			ctn++;
		if (l.getAutoIndex() != true)
			ctn++;
		if (l.getIndex() != "index.html")
			ctn++;
		if (l.isAllowedExtCgi(".py") != true)
			ctn++;
		if (l.getUploadPath() != "/tmp/download")
			ctn++;
		if (l.getErrorPageByCode(404) != "404.html")
			ctn++;
		assertEq("none directive define in location", ctn, 0);
	}
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
		str += "	location /tmp {\n";
		str += "		error_page 401 html/error/401.html\n";
		str += "		index html/index2.html\n";
		str += "		root /tmp\n";
		str += "		autoindex off\n";
		str += "		return 301 /html/index2.html\n";
		str += "		cgi_allowed_ext .cpp\n";
		str += "		upload_path /tmp\n";
		str += "		methods POST\n";
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
		Location	l = vs.getLocationList().front();
		if (l.isAllowedMethod("GET") != true)
			ctn++;
		if (l.isAllowedMethod("POST") != false)
			ctn++;
		if (l.isAllowedMethod("DELETE") != true)
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
		if (l.isAllowedExtCgi(".py") != false)
			ctn++;
		if (l.getUploadPath() != "/download")
			ctn++;
		if (l.getErrorPageByCode(404) != "html/error/404.html")
			ctn++;
		l = vs.getLocationList().back();
		if (l.isAllowedMethod("GET") != false)
			ctn++;
		if (l.isAllowedMethod("POST") != true)
			ctn++;
		if (l.isAllowedMethod("DELETE") != false)
			ctn++;
		if (l.getReturnCode() != 301)
			ctn++;
		if (l.getReturnPath() != "/html/index2.html")
			ctn++;
		if (l.getRoot() != "/tmp")
			ctn++;
		if (l.getAutoIndex() != false)
			ctn++;
		if (l.getIndex() != "html/index2.html")
			ctn++;
		if (l.isAllowedExtCgi(".cpp") != true)
			ctn++;
		if (l.isAllowedExtCgi(".py") != false)
			ctn++;
		if (l.getUploadPath() != "/tmp")
			ctn++;
		if (l.getErrorPageByCode(401) != "html/error/401.html")
			ctn++;
		assertEq("multiple location", ctn, 0);
	}
}
