#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.h"

class Request
{

	public:
		Request(std::string const & toParse);
		~Request();

		//full unparsed request
		std::string rawRequest;

		//requestLine
		std::string method; // GET POST OR DELETE
		std::string url;
		std::string	httpVersion;

		//fieldLines
		std::map<std::string, std::string> fieldLines;
		
		bool		badRequest;

		long	getId(void) const;
	private:
		const	std::string illegalCharacter;
		void		parsingRequest();
		int			parsingRequestLine();
		int			parsingFieldLines();
		static long requestsId; //? use to identify the Request, maybe make it a tab of longs
		long		id;
};

// std::ostream &			operator<<( std::ostream & o, Request const & i );

#endif /* ********************************************************* REQUEST_H */