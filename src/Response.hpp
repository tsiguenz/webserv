#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <sstream>
# include <fstream>
# include <ctime>
# include "Request.hpp"
# include "Response.hpp"
# include "MediaType.hpp"

class Response
{
	public:
		Response( Request  src );
		~Response();

		void		printResponse(void) const;

		std::string method; //  "REQUEST_METHOD="
		std::string url;
		std::string	httpVersion;
		std::map<std::string, std::string> fieldLines; // TT CE DONT TAS BESOIN EST DEDANS
		std::string body;

		std::string			fileName;
		std::vector<char>	file;
		
		std::string 		response;
		int					code; // "REDIRECT_STATUS="

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

#endif /* ******************************************************** RESPONSE_H */
