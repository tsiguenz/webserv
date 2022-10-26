# include "webserv.h"

void	signalHandler(int sig) {
	if (sig == SIGINT)
		throw std::runtime_error("\nServer is closed by user\n");
}

std::string	ltrim(std::string const& s) {
	std::string const	WHITESPACE = " \n\r\t\f\v";
    size_t	start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string	rtrim(std::string const& s) {
	std::string const	WHITESPACE = " \n\r\t\f\v";
    size_t	end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string	trim(std::string const& s) {
    return rtrim(ltrim(s));
}
