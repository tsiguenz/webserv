#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "Location.hpp"
# include <iostream>
# include <list>
# include <map>
# include "webserv.h"

class VirtualServer {
	public:
		// Object managment
		VirtualServer();
		VirtualServer(VirtualServer const& vs);
		virtual ~VirtualServer();

		// Accessors
		std::string				getErrorPageByCode(int const& errorCode) const;
		std::list<std::string>	getServerNames() const;
		std::string				getIp() const;
		short					getPort() const;
		int						getClientMaxBodySize() const;
		std::list<Location>		getLocationList() const;

		void	setErrorPage(int const& errorCode, std::string const& path);
		void	setServerName(std::string const& serverName);
		void	setIp(std::string const& ip);
		void	setPort(short const& port);
		void	setClientMaxBodySize(int const& size);
		void	setNewLocation(Location location);

	private:
		std::list<std::string>	_serverNames;
		std::string				_ip;
		short					_port;
		int						_clientMaxBodySize;
		std::list<Location>		_locationList;
		// set default in locations
		std::map<int, std::string>	_errorPages;
		bool						_autoIndex;
		std::string					_index;
		std::string					_root;
	};

#endif // VIRTUAL_SERVER_HPP
