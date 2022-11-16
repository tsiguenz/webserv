#ifndef WEBSERV_H
# define WEBSERV_H

# include <iostream>
# include <vector>

# define WHITE "\033[0m"
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"

# define BWHITE "\033[1m"
# define BGRAY "\033[1;29m"
# define BBLACK "\033[1;30m"
# define BRED "\033[1;31m"
# define BGREEN "\033[1;32m"
# define BYELLOW "\033[1;33m"
# define BBLUE "\033[1;34m"
# define BPURPLE "\033[1;35m"
# define BCYAN "\033[1;36m"

void		signalHandler(int sig);
std::string	ltrim(const std::string &s);
std::string	rtrim(const std::string &s);
std::string	trim(const std::string &s);
bool        is_digits(const std::string &str);
bool        validateIP(std::string &ip);
bool        isADir(const std::string &url);
std::vector<std::string> split(const std::string &str, char delim);
#endif // WEBSERV_H
