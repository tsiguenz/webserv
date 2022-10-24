#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "Location.hpp"
# include <iostream>
# include <list>
# include <map>

class VirtualServer {
	public:
		// Object managment
		VirtualServer();
		virtual ~VirtualServer();

		// Accessors
		std::string			getErrorPageByCode(int const& errorCode) const;
		std::string			getServerName() const;
		std::string			getIp() const;
		short				getPort() const;
		int					getClientMaxBodySize() const;
		std::list<Location>	getLocationList() const;

	private:
		std::string					_serverName;
		std::string					_ip;
		short						_port;
		std::map<int, std::string>	_errorPages;
		int							_clientMaxBodySize;
		std::list<Location>			_locationList;
	};

#endif // VIRTUAL_SERVER_HPP
