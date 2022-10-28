#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.h"

// class MediaType;

class Request
{
	public:
		Request(std::string const & toParse);
		Request(void);
		Request(const Request & src);
		~Request();


		//full unparsed request
		std::string rawRequest;

		//requestLine
		std::string method;
		std::string url;
		std::string	httpVersion;

		//fieldLines
		std::map<std::string, std::string> fieldLines;

		//body //TODO: adding body if needs, and if body come later, add a isRequestFinishBool and a fnct to add the body later when the rest is coming
		std::string body;

		int			parsingCode;
		
		void		create(std::string const & toParse);
		void		create(Request const & rhs);
		Request &	operator=(Request const & rhs);
		void		printRequest(void);
	private:

		const	std::string illegalCharacter;
		const	std::string escapingCharacter;
		
		// Lexer/Parser
		void			parsingRequest();
		int				parsingRequestLine();
		int				parsingFieldLines();
		int				parsingFieldName(std::string fieldName);
		std::string		parsingFieldValue(std::string fieldValue);
		int				parsingBody(void);

		// Response Checking
		int				checkingFile();
};

#endif /* ********************************************************* REQUEST_H */