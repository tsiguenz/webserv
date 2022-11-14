#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "Location.hpp"
# include <iostream>
# include <list>
# include <map>
# include <vector>
# include "webserv.h"
# include "ErrorHandler.hpp"

class VirtualServer {
	public:
		// Object managment
		VirtualServer();
		VirtualServer(VirtualServer const& vs);
		virtual ~VirtualServer();

		// Accessors
		std::string					getErrorPageByCode(int const& errorCode, std::string const& path = "") const;
		std::map<int, std::string>	getErrorPages(std::string const& path = "") const;
		std::list<std::string>		getServerNames() const;
		std::string					getIp() const;
		short						getPort() const;
		int							getClientMaxBodySize(std::string const& path = "") const;
		std::list<Location>			getLocationList() const;
		std::string					getAutoIndex(std::string const& path = "") const;
		std::string					getRoot(std::string const& path = "") const;
		std::string					getIndex(std::string const& path = "") const;
		std::list<std::string>		getAllowedMethods(std::string const& path = "") const;
		bool						isAllowedMethod(std::string const& method, std::string const& path = "") const;
		std::list<std::string>		getAllowedExtCgi(std::string const& path = "") const;
		bool						isAllowedExtCgi(std::string const& ext, std::string const& path = "") const;
		std::string					getUploadPath(std::string const& path = "") const;
		int							getReturnCode(std::string const& path = "") const;
		std::string					getReturnPath(std::string const& path = "") const;

		std::list<Location>::const_iterator	getLocationByPath(std::string const& path) const;

		void	setErrorPage(int const& errorCode, std::string const& path);
		void	setServerName(std::string const& serverName);
		void	setIp(std::string const& ip);
		void	setPort(short const& port);
		void	setClientMaxBodySize(int const& size);
		void	setNewLocation(Location location);
		void	setAutoIndex(std::string const& autoIndex);
		void	setRoot(std::string const& root);
		void	setIndex(std::string const& index);
		void	setAllowedMethod(std::string const& method);
		void	setAllowedExtCgi(std::string const& ext);
		void	setUploadPath(std::string const& path);
		void	setReturn(int const& code, std::string const& path);
		void	setDefaultValueToLocation();

		ErrorHandler errorHandler;

	private:
		std::list<std::string>		_serverNames;
		std::string					_ip;
		short						_port;
		int							_clientMaxBodySize;
		std::list<Location>			_locationList;
		// set default in locations
		std::map<int, std::string>	_errorPages;
		std::string					_autoIndex;
		std::string					_index;
		std::string					_root;
		std::list<std::string>		_allowedMethods;
		std::list<std::string>		_allowedExtCgi;
		std::string					_uploadPath;
		int							_returnCode;
		std::string					_returnPath;

		std::list<Location>::const_iterator	_getFineLocationIt(std::string const& path) const;
	};

#endif // VIRTUAL_SERVER_HPP
