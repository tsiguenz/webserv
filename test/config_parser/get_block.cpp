#include "test.h"
#include "ConfigParser.hpp"

void	get_block() {
	std::cout << "---------- get_block() ----------\n";
	ConfigParser	cp;
	// basic server block
	{
		int	ctn = 0;
		std::string	str;
		str += "server		    {\n";
		str += "	toto\n";
		str += "}\n";
		std::stringstream	sContent(str);
		std::string ret = cp._getServerBlock(sContent);

		if (ret != str)
			ctn++;

		assertEq("basic server block", ctn, 0);
	}
	// two server blocks
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	toto\n";
		str += "}\n";
		str += "server {\n";
		str += "	tata\n";
		str += "}\n";
		std::stringstream	sContent(str);
		std::string	cmp1 = str.substr(0, str.find("}") + 2);
		std::string	cmp2 = str.substr(str.find("}") + 2);
		std::string ret = cp._getServerBlock(sContent);

		if (ret != cmp1)
			ctn++;
		ret = cp._getServerBlock(sContent);
		if (ret != cmp2)
			ctn++;

		assertEq("two server blocks", ctn, 0);
	}
	// block in block
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	toto\n";
		str += "	location {\n";
		str += "		toto\n";
		str += "	}\n";
		str += "}\n";
		std::stringstream	sContent(str);
		std::string	cmp = str.substr(0, str.find_last_of("}") + 2);
		std::string ret = cp._getServerBlock(sContent);

		if (ret != cmp)
			ctn++;

		assertEq("server block in server block", ctn, 0);
	}
	// empty
	{
		int	ctn = 0;
		std::string	str;
		std::stringstream	sContent(str);
		try { std::string ret = cp._getServerBlock(sContent); }
		catch (std::exception const& e) {
			ctn++;
		}
		assertEq("empty sContent server", ctn, 1);
	}
	// bad head of block
	{
		int	ctn = 0;
		std::string	str;
		str += "servers {\n";
		str += "	toto\n";
		str += "}\n";
		std::stringstream	sContent(str);
		try { std::string ret = cp._getServerBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("bad first line of server block", ctn, 1);
	}
	// non closed server block
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	toto\n";
		std::stringstream	sContent(str);
		try { std::string ret = cp._getServerBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("non closed server block", ctn, 1);
	}
	// server block non closed in server block
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	toto\n";
		str += "	location {\n";
		str += "		toto\n";
		str += "}\n";
		std::stringstream	sContent(str);
		std::string	cmp = str.substr(0, str.find_last_of("}") + 2);

		try { std::string ret = cp._getServerBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}

		assertEq("server block non closed in server block", ctn, 1);
	}
	// two server blocks with one non closed
	{
		int	ctn = 0;
		std::string	str;
		str += "server {\n";
		str += "	toto\n";
		str += "}\n";
		str += "									\n";
		str += "server {\n";
		std::stringstream	sContent(str);
		std::string	cmp = str.substr(0, str.find("}") + 2);
		std::string ret = cp._getServerBlock(sContent);

		if (ret != cmp)
			ctn++;
		try { std::string ret = cp._getServerBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("two server blocks whith one non closed", ctn, 1);
	}
	// basic location block
	{
		int	ctn = 0;
		std::string	str;
		str += "location /html  {\n";
		str += "	toto\n";
		str += "}\n";
		std::stringstream	sContent(str);
		std::string ret = cp._getLocationBlock(sContent);

		if (ret != str)
			ctn++;

		assertEq("basic location block", ctn, 0);
	}
	// bad format head of location block
	{
		int	ctn = 0;
		std::string	str;
		str += "location  {\n";
		str += "	toto\n";
		str += "}\n";
		std::stringstream	sContent(str);
		
		try { std::string ret = cp._getLocationBlock(sContent); }
		catch (std::exception const& e) {
			ctn++;
		}
		assertEq("location block with no path", ctn, 1);
	}
	// two location blocks
	{
		int	ctn = 0;
		std::string	str;
		str += "location  /html {\n";
		str += "	toto\n";
		str += "}\n";
		str += "location /html {\n";
		str += "	tata\n";
		str += "}\n";
		std::stringstream	sContent(str);
		std::string	cmp1 = str.substr(0, str.find("}") + 2);
		std::string	cmp2 = str.substr(str.find("}") + 2);
		std::string ret = cp._getLocationBlock(sContent);

		if (ret != cmp1)
			ctn++;
		ret = cp._getLocationBlock(sContent);
		if (ret != cmp2)
			ctn++;

		assertEq("two location blocks", ctn, 0);
	}
	// block in block
	{
		int	ctn = 0;
		std::string	str;
		str += "location /html {\n";
		str += "	toto\n";
		str += "	location /html {\n";
		str += "		toto\n";
		str += "	}\n";
		str += "}\n";
		std::stringstream	sContent(str);
		try { std::string ret = cp._getLocationBlock(sContent); }
		catch (std::exception const& e) {
			ctn++;
		}

		assertEq("location block in location block is error", ctn, 1);
	}
	// empty
	{
		int	ctn = 0;
		std::string	str;
		std::stringstream	sContent(str);
		try { std::string ret = cp._getLocationBlock(sContent); }
		catch (std::exception const& e) {
			ctn++;
		}
		assertEq("empty sContent location", ctn, 1);
	}
	// bad head of block
	{
		int	ctn = 0;
		std::string	str;
		str += "locations /html {\n";
		str += "	toto\n";
		str += "}\n";
		std::stringstream	sContent(str);
		try { std::string ret = cp._getLocationBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("bad first line of location block", ctn, 1);
	}
	// non closed location block
	{
		int	ctn = 0;
		std::string	str;
		str += "location /html {\n";
		str += "	toto\n";
		std::stringstream	sContent(str);
		try { std::string ret = cp._getLocationBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("non closed location block", ctn, 1);
	}
	// two location blocks with one non closed
	{
		int	ctn = 0;
		std::string	str;
		str += "location /home {\n";
		str += "	toto\n";
		str += "}\n";
		str += "									\n";
		str += "location {\n";
		std::stringstream	sContent(str);
		std::string	cmp = str.substr(0, str.find("}") + 2);
		std::string ret = cp._getLocationBlock(sContent);

		if (ret != cmp)
			ctn++;
		try { std::string ret = cp._getLocationBlock(sContent); }
		catch (std::exception const& e) {
			(void) e;
			ctn++;
		}
		assertEq("two location blocks whith one non closed", ctn, 1);
	}
}
