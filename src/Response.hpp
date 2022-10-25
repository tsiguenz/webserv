#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.h"

class Request;
// class MediaType;

class Response
{
	public:

		Response( Request  src );
		~Response();

		std::string method;
		std::string url;
		std::string	httpVersion;
		std::map<std::string, std::string> fieldLines;
		std::string body;

		std::vector<char>	file;
		std::string 		response;
		int					code;

	private:
		
		void		buildingResponse(void);
		void		getFile(void);
		
		// const MediaType	mime;
		std::string	getTime(void);
		std::string getLength(void);
		void	getTypeContent(void);

};

// std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */