#ifndef WEBSERV_H
# define WEBSERV_H

# include <errno.h>

# include <unistd.h>		// close()
# include <stdlib.h>		// exit()
# include <strings.h>		// bzero()
# include <fcntl.h>		// bzero()
# include <sys/socket.h> 	// socket(), bind()
# include <sys/types.h>		// exit()
# include <sys/epoll.h>		// epoll()
# include <netinet/in.h>	// struct sockaddr_in
# include <arpa/inet.h>		// inet_addr()
# include <iostream>
# include <vector>

typedef struct epoll_event	epoll_event;
typedef struct sockaddr_in	sockaddr_in;
typedef struct sockaddr		sockaddr;

void	ft_exit(std::string str, int ret);
void	ft_exit(std::string str, int ret, int serverSocket);

#endif // WEBSERV_H
