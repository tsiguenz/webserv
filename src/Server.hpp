#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <iostream>
# include <cstdlib> 		// isdigit()
# include <unistd.h>		// close()
# include <stdlib.h>		// exit()
# include <strings.h>		// bzero()
# include <fcntl.h>			// fcntl()
# include <sys/socket.h> 	// socket(), bind()
# include <sys/types.h>		// exit()
# include <sys/epoll.h>		// epoll()
# include <netinet/in.h>	// struct sockaddr_in
# include <arpa/inet.h>		// inet_addr()
# include "Response.hpp"
# include "webserv.h"
# include "ConfigParser.hpp"

# define BUFFER_SIZE 4096
# define EVENTS_MAX 1000
# define BACKLOG 10

class Server {
	public:
		typedef struct epoll_event	epoll_event;
		typedef struct sockaddr_in	sockaddr_in;
		typedef struct sockaddr		sockaddr;

		Server(ConfigParser const& cp);
		virtual ~Server();
		void	run();
		std::vector<int>	getFdsServer() const;
		std::vector<int>	getFdsclient() const;

	private:
		std::map<int, Request>		_requests;
		std::vector<int>			_fdsServer;
		std::list<VirtualServer>	_virtualServerList;
		int							_epFd;
		epoll_event					_events[EVENTS_MAX];

		// Member functions
		void	_initEpoll();
		void	_initVirtualServer(VirtualServer const& vs);
		void	_setNonBlocking(int const& fd) const;
		void	_addEvent(int const& fd, long const& events) const;
		void	_modEvent(int const& fd, long const& events) const;
		void	_delEvent(int const& fd) const;
		bool	_isServer(int const& fd) const;

		// Main loop who handle I/O events
		void	_newConnection(int const& fd);
		void	_handleEvent(int const& nbEpollFd);
		void	_parseRequest(epoll_event const& event);
		void	_sendResponse(epoll_event const& event);
};

#endif // SERVER_HPP
