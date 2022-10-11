#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.h"

# define BUFFER_SIZE 1064 * 128
# define EVENTS_MAX 100
# define BACKLOG 10
# define DUMMY_RESPONSE "HTTP/1.1 200 OK\nServer: test\nContent-Lenght: 13\nContent-Type: text/html\n\n<H1>webserv</H1>\n"

class Server {
public:
	typedef struct epoll_event	epoll_event;
	typedef struct sockaddr_in	sockaddr_in;
	typedef struct sockaddr		sockaddr;

	// TODO change that for the struct returned by the parsing of file config
	Server(int const& port);

	virtual ~Server();

	void	run();

private:
	// TODO create struct for that to handle multiple ports
	int _port; // const ?
	int	_epFd; // const ?
	int _fdServer; // const ?
	epoll_event	_events[EVENTS_MAX];

	void	_initServer();
	void	_addEvent(int& fd, long events);
	void	_modEvent(int& fd, long events);
	void	_delEvent(int& fd);
};

#endif // SERVER_HPP
