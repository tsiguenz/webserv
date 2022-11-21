# include <stdexcept>
# include <csignal>
# include <vector>
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <dirent.h>

void	signalHandler(int sig) {
	if (sig == SIGINT)
		throw std::runtime_error("\nServer is closed by user");
	if (sig == SIGPIPE)
        throw std::runtime_error("Server catch SIGPIPE");
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

bool is_digits(const std::string &str)
{
    return !str.empty() && (str.find_first_not_of("0123456789") == std::string::npos);
}

 
std::vector<std::string> split(const std::string &str, char delim)
{
    size_t i = 0;
    std::vector<std::string> list;
 
    size_t pos = str.find(delim);
 
    while (pos != std::string::npos)
    {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }
 
    list.push_back(str.substr(i, str.length()));
 
    return list;
}

bool validateIP(std::string & ip)
{
    int value;
    std::vector<std::string> list = split(ip, '.');
 
    if (list.size() != 4) {
        return false;
    }

    std::vector<std::string>::iterator it = list.begin();
    std::vector<std::string>::iterator end = list.end();

    for (; it != end; it++)
    {
        if ((*it).size() > 3)
            return false;
        value = atoi((*it).c_str());
        if (!is_digits((*it)) || value > 255 || value < 0)  {
            return false;
        }
        // if (!is_digits((*it)) || std::stoi((*it)) > 255 || std::stoi((*it)) < 0) {
        //     return false;
        // }
    }
 
    return true;
}

bool isADir(const std::string & url) {
    DIR *d;
    d = opendir(url.c_str());
    if (d == NULL)
        return 0;
    closedir(d);
    return 1;
}
