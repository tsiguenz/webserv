#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.h"

class Request
{

	public:
		Request(std::string const & toParse);
		~Request();


	private:
		std::string raw; //the full unparsed request
		
		std::string type; // GET POST OR DELETE
		std::string	Host;
		std::string	Accepted_format;
		std::string	Connection;


		static long Request_ID; //? use to identify the Request, maybe make it a tab of longs
};

std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */