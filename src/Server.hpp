#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.h"

# define BUFFER_SIZE 1024 // ?
# define EVENTS_MAX 1000 // ?
# define BACKLOG 10
# define DUMMY_RESPONSE "HTTP/1.1 200 OK\nServer: test\nContent-Lenght: 13\nContent-Type: text/html\n\n<H1>webserv</H1>\n"

class Request;

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
		// TODO create struct for that to handle multiple virtual servers
		std::vector<int>	_fdsServer;
		// TODO keep alive need to store clients for next I/O operations
		std::vector<int>	_fdsClient;
		int					_epFd;
		epoll_event			_events[EVENTS_MAX];

		void	_initEpoll();

		// Initialize new socket (fd) who can listen on a specific port
		void	_initVirtualServer(int const& port);
		void	_addEvent(int const& fd, long const& events) const;
		void	_modEvent(int const& fd, long const& events) const;
		void	_delEvent(int const& fd) const;
		bool	_isServer(int const& fd) const;

		// Main loop who handle I/O events
		void	_newConnection(int const& fd) const;
		void	_handleEvent(int const& nbEpollFd) const;
		void	_parseRequest(epoll_event const& event) const;
		void	_sendResponse(epoll_event const& event) const;

		void	_setNonBlocking(int const& fd) const;
};

#endif // SERVER_HPP
