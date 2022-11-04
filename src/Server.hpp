#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <iostream>
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

# define BUFFER_SIZE 1024 // ?
# define EVENTS_MAX 1000 // ?
# define BACKLOG 10
# define DUMMY_RESPONSE "HTTP/1.1 200 OK\r\nServer: test\r\nContent-Lenght: 17\r\nContent-Type: text/html\r\n\r\n<H1>webserv</H1>\r\n"

class Request;
class Response;


class Server {
	public:
		typedef struct epoll_event	epoll_event;
		typedef struct sockaddr_in	sockaddr_in;
		typedef struct sockaddr		sockaddr;

		// TODO change that for the struct returned by the parsing of file config
		Server(int const& port);
		virtual ~Server();
		void	run();
		std::vector<int>	getFdsServer() const;
		std::vector<int>	getFdsclient() const;
	private:
		// TODO replace by class Parser
		std::vector<int>	_fdsServer;
		// TODO keep alive need to store clients for next I/O operations
		int					_epFd;
		epoll_event			_events[EVENTS_MAX];
		Request				currentRequest();

		void	_initEpoll();

		// Initialize new socket (fd) who can listen on a specific port
		void	_initVirtualServer(int const& port);
		void	_setNonBlocking(int const& fd) const;
		void	_addEvent(int const& fd, long const& events) const;
		void	_modEvent(int const& fd, long const& events) const;
		void	_delEvent(int const& fd) const;
		bool	_isServer(int const& fd) const;

		// Main loop who handle I/O events
		void	_newConnection(int const& fd) const;
		void	_handleEvent(int const& nbEpollFd) const;
		Request	_parseRequest(epoll_event const& event) const;
		void	_sendResponse(epoll_event const& event, Response const& currentResponse) const;
};

#endif // SERVER_HPP
