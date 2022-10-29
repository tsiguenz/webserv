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
		assertEq("server name", ctn, 2);
	}
	// parse listen
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
	// error page
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
		assertEq("error page", ctn, 1);
	}
	// client max body size
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
			try { cp._parseDirective("	client_max_body	10485761 sadfasdf", vs); }
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
}
