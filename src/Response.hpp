#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <sstream>
# include <fstream>
# include <ctime>
# include <sstream> 
# include "Request.hpp"
# include "MediaType.hpp"
# include "VirtualServer.hpp"
# include "Cgi.hpp"

class Response
{
	public:
		Response();
		Response( Request  src, VirtualServer const & server);
		~Response();

		void		printResponse(void);

		std::string							method; //  "REQUEST_METHOD=" //x
		std::string							url;
		std::string							httpVersion; //x
		std::map<std::string, std::string>	fieldLines; // TT CE DONT TAS BESOIN EST DEDANS
		std::vector<char>					body; //x
		std::string							root; //pour adam //x
		std::string							serverName; //pour adam //x
		std::string							port; //pour adam
		std::string							fileName;
		std::vector<char>					file;
		std::string 						response;
		int									code; // "REDIRECT_STATUS="

	private:
		MediaType											mime;
		VirtualServer										server;
		std::map<int, std::pair<std::string, std::string> >	statusCodes;

		void		buildingResponse(void);
		void		getFile(void); //GET
		void		postFile(void); //POST
		void		deleteFile(void); //DELETE

		void	checkingMethod(void);
		void	handleError(void);
		void	redirectionUrl(void);

		std::string autoindex_nocgi(void);
		std::string getResponse(void);
		std::string	getTime(void);
		std::string	getServerName(void);
		std::string getLength(void);
		std::string	getConnectionType(void);
		std::string	getTypeContent(void);
};

#endif /* ******************************************************** RESPONSE_H */
