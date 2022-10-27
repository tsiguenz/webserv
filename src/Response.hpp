#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.h"

# include "Request.hpp"
# include "Response.hpp"
# include "MediaType.hpp"
// # define <!DOCTYPE html><html><head><title>Code d'erreur</title></head><body><a href="lien"></a><p>message</p></body></html>
class Request;
class MediaType;

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

		std::string			fileName;
		std::vector<char>	file;
		
		std::string 		response;
		int					code;

		void		printResponse(void) const;
	private:
		
		MediaType	mime;
		std::map<int, std::pair<std::string, std::string> >	statusCodes;

		void		buildingResponse(void);
		
		void		getFile(void); //GET
		void		postFile(void); //POST
		void		deleteFile(void); //DELETE
		
		void	handleError(void);
		void	redirectionUrl(void);

		std::string getResponse(void);
		std::string	getTime(void);
		std::string	getServerName(void);
		std::string getLength(void);
		std::string	getConnectionType(void);
		std::string	getTypeContent(void);

		void		initMapCode(void);
};

// std::ostream &			operator<<( std::ostream & o, Response const & i );

#endif /* ******************************************************** RESPONSE_H */
