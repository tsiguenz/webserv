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
		Response( Request  src);
		Response(Response const& rhs);
		Response&	operator=(Response const& rhs);
		~Response();

		void		printResponse(void) const;

		std::string							method; //  "REQUEST_METHOD=" //x
		std::string							url;
		std::string							httpVersion; //x
		std::map<std::string, std::string>	fieldLines; // TT CE DONT TAS BESOIN EST DEDANS
		std::vector<unsigned char>			body;
		std::string							root; //pour adam
		std::string							serverName; //pour adam
		std::string							port; //pour adam
		std::string							queryString; //pour adam
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

		// POST
		void	checkingMethod(void);
		void	handleError(void);
		void	redirectionUrl(void);
		void	_postFormData();
		void	_postFormDataBlock();
		std::vector<unsigned char>	_getFormDataBlock();
		void	_postFormDataBlock(std::vector<unsigned char> const& v);

		void		redirectionIndex(void);
		bool		isAutoIndex;
		std::string autoindex_nocgi(void);
		std::string getResponse(void);
		std::string	getTime(void);
		std::string	getServerName(void);
		std::string getLength(void);
		std::string	getConnectionType(void);
		std::string	getTypeContent(void);
		std::string auto_index(char **env);
		std::string	handleErrorCgi(void);

		template<typename It>
			size_t	_itFind(It const& begin, It const& end, std::string const& toSearch, size_t pos = 0) {
				if (pos > (size_t) std::distance(begin, end) || toSearch.empty() == true)
					return std::string::npos;
				for (It it = begin + pos; it != end; it++) {
					if (*it != toSearch[0])
						continue ;
					for (size_t i = 0; i < toSearch.size(); i++) {
						if (*(it + i) != toSearch[i])
							break ;
						if (i == toSearch.size() - 1)
							return std::distance(begin, it);
					}
				}
				return std::string::npos;
			}
};

#endif /* ******************************************************** RESPONSE_H */
