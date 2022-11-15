#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <sstream>
# include <fstream>
# include <ctime>
# include <sstream> 
# include "Request.hpp"
# include "Response.hpp"
# include "MediaType.hpp"
# include "VirtualServer.hpp"

class Response
{
	public:
		Response();
		Response( Request  src, VirtualServer const & server);
		~Response();

		void		printResponse(void) const;

		std::string							method; //  "REQUEST_METHOD="
		std::string							url;
		std::string							httpVersion;
		std::map<std::string, std::string>	fieldLines; // TT CE DONT TAS BESOIN EST DEDANS
		std::vector<char>					body;
		std::string							root; //pour adam
		std::string							serverName; //pour adam
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
		void	_postFormData();
		void	_postFormDataBlock();
		std::vector<char>	_getFormDataBlock();
//		size_t	_bodyFind(std::string const& toSearch, size_t pos = 0);
		void	_postFormDataBlock(std::vector<char> const& v);

		std::string getResponse(void);
		std::string	getTime(void);
		std::string	getServerName(void);
		std::string getLength(void);
		std::string	getConnectionType(void);
		std::string	getTypeContent(void);

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
