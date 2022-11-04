#include "test.h"
#include "ConfigParser.hpp"

void	parse_directive() {
	std::cout << "---------- parse_directive() ----------\n";
	ConfigParser	cp;
	// parse server name
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("   		server_name 		yoyo     ", vs);
			if (vs.getServerNames().front() != "yoyo")
				ctn++;
		}
		{
			VirtualServer	vs;
			cp._parseDirective("server_name yoyo", vs);
			if (vs.getServerNames().front() != "yoyo")
				ctn++;
		}
		{
			VirtualServer	vs;
			try { cp._parseDirective("server_name", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		{
			VirtualServer	vs;
			try { cp._parseDirective("server_name ", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		{
			VirtualServer	vs;
			cp._parseDirective("   		server_name 		yoyo eho.com     ", vs);
			if (vs.getServerNames().front() != "yoyo")
				ctn++;
			if (vs.getServerNames().back() != "eho.com")
				ctn++;
		}
		{
			VirtualServer	vs;
			cp._parseDirective("server_name 		yoyo eho.com google.com www.webserv.fr", vs);
			std::list<std::string>	sv = vs.getServerNames();
			std::list<std::string>::const_iterator	it = sv.begin();
			if (*it++ != "yoyo")
				ctn++;
			if (*it++ != "eho.com")
				ctn++;
			if (*it++ != "google.com")
				ctn++;
			if (*it++ != "www.webserv.fr")
				ctn++;
		}
		{
			VirtualServer	vs;
			cp._parseDirective("server_name	eho.com", vs);
			if (vs.getServerNames().front() != "eho.com")
				ctn++;
		}
		assertEq("server_name", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	listen	127.0.0.5:4242	", vs);
			if (vs.getIp() != "127.0.0.5")
				ctn++;
			if (vs.getPort() != 4242)
				ctn++;
		}
		{
			VirtualServer	vs;
			cp._parseDirective("listen	127.0.0.5", vs);
			if (vs.getIp() != "127.0.0.5")
				ctn++;
			if (vs.getPort() != 0)
				ctn++;
		}
		{
			VirtualServer	vs;
			cp._parseDirective("listen	4242", vs);
			if (vs.getIp() != "")
				ctn++;
			if (vs.getPort() != 4242)
				ctn++;
		}
		{
			VirtualServer	vs;
			try { cp._parseDirective("listen", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		{
			VirtualServer	vs;
			try { cp._parseDirective("listen 8080 yoyo", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("listen", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	error_page	404 404.html	", vs);
			if (vs.getErrorPageByCode(404) != "404.html")
				ctn++;
		}
		{
			VirtualServer	vs;
			cp._parseDirective("	error_page	404 412 404.html	", vs);
			if (vs.getErrorPageByCode(404) != "404.html")
				ctn++;
			if (vs.getErrorPageByCode(412) != "404.html")
				ctn++;
		}
		{
			VirtualServer	vs;
			try { cp._parseDirective("	error_page		", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("error_page", ctn, 1);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	client_max_body	42	", vs);
			if (vs.getClientMaxBodySize() != 42)
				ctn++;
			try { cp._parseDirective("	client_max_body		", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	client_max_body	-1	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			// overflow
			try { cp._parseDirective("	client_max_body	5555555555", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	client_max_body	1048577 sadfasdf", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	client_max_body	sadfasdf", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("client_max_body", ctn, 5);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	autoindex	on	", vs);
			if (vs.getAutoIndex() != "on")
				ctn++;
			cp._parseDirective("	autoindex	off	", vs);
			if (vs.getAutoIndex() != "off")
				ctn++;
			try { cp._parseDirective("	autoindex	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	autoindex	on off", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	autoindex	offf", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("autoindex", ctn, 3);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	root	/var/www	", vs);
			if (vs.getRoot() != "/var/www")
				ctn++;
			try { cp._parseDirective("	root	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	root	/var/www /var/html", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("root", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	index	index.html	", vs);
			if (vs.getIndex() != "index.html")
				ctn++;
			try { cp._parseDirective("	index	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	index	index.html html/index.html", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("index", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	methods	GET	", vs);
			if (vs.getAllowedMethods().front() != "GET")
				ctn++;
			cp._parseDirective("	methods	GET	DELETE POST	", vs);
			std::list<std::string>					lm = vs.getAllowedMethods();
			std::list<std::string>::const_iterator	it = lm.begin();
			if (*it++ != "GET")
				ctn++;
			if (*it++ != "DELETE")
				ctn++;
			if (*it++ != "POST")
				ctn++;
			try { cp._parseDirective("	methods	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	methods	get", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("methods", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	return	404 http://google.com", vs);
			if (vs.getReturnCode() != 404)
				ctn++;
			if (vs.getReturnPath() != "http://google.com")
				ctn++;
			try { cp._parseDirective("	return	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	return	404 www.google.com test", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	return	0 www.google.com test", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("return", ctn, 3);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	cgi_allowed_ext	.php .py", vs);
			cp._parseDirective("	cgi_allowed_ext	.py", vs);
			std::list<std::string>					lm = vs.getAllowedExtCgi();
			if (lm.front() != ".php")
				ctn++;
			if (lm.back() != ".py")
				ctn++;
			if (lm.size() != 2)
				ctn++;
			try { cp._parseDirective("	cgi_allowed_ext	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	cgi_allowed_ext	py", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("cgi_allowed_ext", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("	upload_path	download", vs);
			if (vs.getUploadPath() != "download")
				ctn++;
			try { cp._parseDirective("	upload_path	", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
			try { cp._parseDirective("	upload_path	abc def", vs); }
			catch (std::exception const& e) {
				(void) e;
				ctn++;
			}
		}
		assertEq("upload_path", ctn, 2);
	}
}
