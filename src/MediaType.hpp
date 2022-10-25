#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include "webserv.h"

class MediaType
{

	public:

		MediaType();
		~MediaType();

		bool 		isInAccepted(std::string requestedTypes, std::string fileFormat); //(recoit la ligne complete de "Accepted: " et la parse (du coup fait les ordre de prio et tout)) et check si ce quon sapprete a renvoyer et dans accepted //TODO
		std::string	isTypeSupported(std::string	requestedType); //recoit la ligne dans contentType et check si le type est bien un media type accepté //TODO


		std::string getMediaType(std::string requestedType);
	private:
		std::map<std::string, std::string> mime;

};


#endif /* ******************************************************* MEDIATYPE_H */