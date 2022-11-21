#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.h"
# include <map>
# include <vector>
# include <sys/types.h>

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
		std::string	queryString;

		//fieldLines
		std::map<std::string, std::string> fieldLines;
		bool	isRequestComplete;
		bool	isParsingComplete;
		size_t	requestLen;
		std::vector<unsigned char> body;

		int			parsingCode;
		void		addingBuffer(std::vector<unsigned char> toAdd, ssize_t const& len);
		void		create(std::vector<unsigned char> toParse2);
		void		create(Request const & rhs);
		Request &	operator=(Request const & rhs);
		void		printRequest(void);
	private:

		const	std::string illegalCharacter;
		const	std::string escapingCharacter;
		size_t 	posEnd;
		// Parser
		bool			preParsing();
		void			parsingRequest();
		int				parsingRequestLine();
		int				definingBody();
		void			completingBody();
		int				parsingFieldLines();
		int				parsingFieldName(std::string fieldName);
		std::string		parsingFieldValue(std::string fieldValue);
		int				parsingBody();
		int				parsingBody2();

		void			trimingFieldLines();

		// Response Checking
		int				checkingFile();
};

#endif /* ********************************************************* REQUEST_H */
