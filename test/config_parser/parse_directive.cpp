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
		assertEq("server name tests", ctn, 2);
	}
	{
		int	ctn = 0;
		{
			VirtualServer	vs;
			cp._parseDirective("listen	127.0.0.5:4242", vs);
			if (vs.getIp() != "127.0.0.5")
				ctn++;
			if (vs.getPort() != 4242)
				ctn++;
		}
		assertEq("listen tests", ctn, 0);
	}
}
