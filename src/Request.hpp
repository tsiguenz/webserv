#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.h"
# include <map>
# include <vector>
# include <sys/types.h>
# include "VirtualServer.hpp"

// class MediaType;

class Request
{
	public:
		Request();
		Request(std::vector<unsigned char> & toParse);
		Request(std::list<VirtualServer>	 & virtualServerList);
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
		std::list<VirtualServer>	_virtualServerList;
		VirtualServer	server;
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
		void			definingServer();
		void			completingBody();
		int				parsingFieldLines();
		int				parsingFieldName(std::string fieldName);
		std::string		parsingFieldValue(std::string fieldValue);
		int				parsingBody();

		void			trimingFieldLines();

		VirtualServer 	_getVirtualServerByHost() const;
		VirtualServer 	_selectServer(short const& port, std::string const& ip, std::string const& name) const;

		// Response Checking
		int				checkingFile();
};

#endif /* ********************************************************* REQUEST_H */
