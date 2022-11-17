#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.h"
# include <map>
# include <vector>

// class MediaType;

class Request
{
	public:
		Request(std::vector<unsigned char> & toParse);
		Request(void);
		// Request(const Request & src);
		~Request();


		//full unparsed request
		std::vector<unsigned char>	vectorRequest;
		std::string			rawRequest;
		//requestLine
		std::string method;
		std::string url;
		std::string	httpVersion;

		//fieldLines
		std::map<std::string, std::string> fieldLines;
		bool	isRequestComplete;
		bool	isParsingComplete;
		size_t	requestLen;
		//body //TODO: adding body if needs, and if body come later, add a isRequestFinishBool and a fnct to add the body later when the rest is coming
		//TODO VECTOR UNSIGNED CHAR
		std::vector<unsigned char> body;

		int			parsingCode;
		void		addingBuffer(std::vector<unsigned char> toAdd );
		void		create(std::vector<unsigned char> toParse2);
		void		create(Request const & rhs);
		Request &	operator=(Request const & rhs);
		void		printRequest(void);
	private:

		const	std::string illegalCharacter;
		const	std::string escapingCharacter;
		size_t 	posEnd;
		// Lexer/Parser
		int				preParsing(void);
		void			parsingRequest();
		int				parsingRequestLine();
		int				definingBody();
		void			completingBody();
		int				parsingFieldLines();
		int				parsingFieldName(std::string fieldName);
		std::string		parsingFieldValue(std::string fieldValue);
		int				parsingBody(void);
		int				parsingBody2(void);

		void			trimingFieldLines();

		// Response Checking
		int				checkingFile();
};

#endif /* ********************************************************* REQUEST_H */
