#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include "webserv.h"

typedef struct s_data {
	bool	isCGI;
	std::string path;
	std::string mimeType;
}	t_data;

class MediaType {

	public:

		MediaType();
		MediaType(MediaType const& mt);
		MediaType&	operator=(MediaType const& rhs);
		~MediaType();

		std::string getMediaType(std::string requestedExtension);
		std::string getMediaExtension(std::string requestedType);
		bool 		isInAccepted(std::string accepted, std::string fileFormat);
		bool		isTypeSupported(std::string	requestedType);
		bool		isExtensionSupported(std::string requestedExtension);
		void		add(std::string s, bool isCgi, std::string path, std::string mimeType);
		std::string getProgName(std::string &extension);
		bool		isCgi(std::string extension);

	private:
		std::map<std::string, t_data> mime;

};


#endif /* ******************************************************* MEDIATYPE_H */
