#include "test.h"
#include "ConfigParser.hpp"

void	parse_location_block() {
	std::cout << "---------- parse_location_block() ----------\n";
	ConfigParser	cp;
	{
		int					ctn = 0;
		Location			l;
		std::string			locationBlock;
		locationBlock += "location /html {\n";
		locationBlock += "}\n";
		l = cp._parseLocationBlock(locationBlock);
		if (l.getAllowedMethods().empty() != true)
			ctn++;
		if (l.getReturnCode() != 0)
			ctn++;
		if (l.getReturnPath().empty() != true)
			ctn++;
		if (l.getRoot().empty() != true)
			ctn++;
		if (l.getAutoIndex() != false)
			ctn++;
		if (l.getIndex().empty() != true)
			ctn++;
		if (l.getAllowedExtCgi().empty() != true)
			ctn++;
		if (l.getUploadPath().empty() != true)
			ctn++;
		if (l.getErrorPages().empty() != true)
			ctn++;
		assertEq("empty location block", ctn, 0);
	}
	{
		int					ctn = 0;
		Location			l;
		std::string			locationBlock;
		locationBlock += "location /html {\n";
		locationBlock += "	methods GET\n";
		locationBlock += "	return 301 /html/index.html\n";
		locationBlock += "	root /tmp/www\n";
		locationBlock += "	autoindex on\n";
		locationBlock += "	index html/index.html\n";
		locationBlock += "	cgi_allowed_ext .php\n";
		locationBlock += "	upload_path /download\n";
		locationBlock += "	error_page 404 html/error/404.html\n";
		locationBlock += "}\n";
		l = cp._parseLocationBlock(locationBlock);
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
		assertEq("basic valid location block", ctn, 0);
	}
}
