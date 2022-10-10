#ifndef WEBSERV_H
# define WEBSERV_H

# include <errno.h>

# include <unistd.h>		// close()
# include <stdlib.h>		// exit()
# include <strings.h>		// bzero()
# include <sys/socket.h> 	// socket(), bind()
# include <sys/types.h>		// exit()
# include <netinet/in.h>	// struct sockaddr_in
# include <arpa/inet.h>		// inet_addr()
# include <iostream>
# include <vector>

void	ft_exit(std::string str, int ret);
void	ft_exit(std::string str, int ret, std::vector<int> socketFd);

#endif // WEBSERV_H
