#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "Location.hpp"

class VirtualServer {
	public:
		// Object managment
		VirtualServer();
		virtual ~VirtualServer();

		// Getter
		std::string	getServerName();
		std::string	getIp();
		short		getPort();
		std::string	getErrorPage();
		std::string	getClientMaxBodySize();
		std::vector<Location>	getLocationList();

	private:
		std::string			_serverName;
		std::string			_ip;
		short				_port;
		std::string			_errorPage;
		std::string			_clientMaxBodySize;
		std::vector<Location>	_locationList;
	};

#endif // VIRTUAL_SERVER_HPP
