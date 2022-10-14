#ifndef WEBSERV_H
# define WEBSERV_H

# include <errno.h>

# include <unistd.h>		// close()
# include <stdlib.h>		// exit()
# include <strings.h>		// bzero()
# include <fcntl.h>			// bzero()
# include <sys/socket.h> 	// socket(), bind()
# include <sys/types.h>		// exit()
# include <sys/epoll.h>		// epoll()
# include <netinet/in.h>	// struct sockaddr_in
# include <arpa/inet.h>		// inet_addr()
# include <iostream>
# include <csignal>			// signal()
# include <vector>
# include <map>
# include <stdexcept>

# include "Server.hpp"
# include "Request.hpp"

void	signalHandler(int sig);

#endif // WEBSERV_H
