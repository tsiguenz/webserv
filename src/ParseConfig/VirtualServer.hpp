#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "Location.hpp"
# include <iostream>
# include <list>
# include <map>
# include <vector>
# include "webserv.h"

class VirtualServer {
	public:
		// Object managment
		VirtualServer();
		VirtualServer(VirtualServer const& vs);
		virtual ~VirtualServer();

		// Accessors
		std::string					getErrorPageByCode(int const& errorCode) const;
		std::map<int, std::string>	getErrorPages() const;
		std::list<std::string>		getServerNames() const;
		std::string					getIp() const;
		short						getPort() const;
		int							getClientMaxBodySize() const;
		std::list<Location>			getLocationList() const;
		bool						getAutoIndex() const;
		std::string					getRoot() const;
		std::string					getIndex() const;
		std::list<std::string>		getAllowedMethods() const;
		bool						isAllowedMethod(std::string const& method) const;
		std::list<std::string>		getAllowedExtCgi() const;
		bool						isAllowedExtCgi(std::string const& ext) const;
		std::string					getUploadPath() const;
		int							getReturnCode() const;
		std::string					getReturnPath() const;

		void	setErrorPage(int const& errorCode, std::string const& path);
		void	setServerName(std::string const& serverName);
		void	setIp(std::string const& ip);
		void	setPort(short const& port);
		void	setClientMaxBodySize(int const& size);
		void	setNewLocation(Location location);
		void	setAutoIndex(bool const& autoIndex);
		void	setRoot(std::string const& root);
		void	setIndex(std::string const& index);
		void	setAllowedMethod(std::string const& method);
		void	setAllowedExtCgi(std::string const& ext);
		void	setUploadPath(std::string const& path);
		void	setReturn(int const& code, std::string const& path);

	private:
		std::list<std::string>		_serverNames;
		std::string					_ip;
		short						_port;
		int							_clientMaxBodySize;
		std::list<Location>			_locationList;
		// set default in locations
		std::map<int, std::string>	_errorPages;
		bool						_autoIndex;
		std::string					_index;
		std::string					_root;
		std::list<std::string>		_allowedMethods;
		std::list<std::string>		_allowedExtCgi;
		std::string					_uploadPath;
		int							_returnCode;
		std::string					_returnPath;
	};

#endif // VIRTUAL_SERVER_HPP
