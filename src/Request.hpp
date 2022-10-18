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
		std::string method;
		std::string url;
		std::string	httpVersion;

		//fieldLines
		std::map<std::string, std::string> fieldLines;

		//body
		std::string body;

		bool		badRequest;

	private:

		const	std::string illegalCharacter;
		const	std::string escapingCharacter;
		
		void		parsingRequest();
		int			parsingRequestLine();
		int			parsingFieldLines();
		int			parsingFieldName(std::string fieldName);
		std::string		parsingFieldValue(std::string fieldValue);

};

#endif /* ********************************************************* REQUEST_H */