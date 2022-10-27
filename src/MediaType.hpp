#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include "webserv.h"

class MediaType
{

	public:

		MediaType();
		~MediaType();

		bool 	isInAccepted(std::string requestedTypes, std::string fileFormat); //(recoit la ligne complete de "Accepted: " et la parse (du coup fait les ordre de prio et tout)) et check si ce quon sapprete a renvoyer et dans accepted //TODO
		bool	isTypeSupported(std::string	requestedType); //pour post 
		bool	isExtensionSupported(std::string	requestedExtension);

		std::string getMediaExtension(std::string requestedType);
		std::string getMediaType(std::string requestedType);//recoit la ligne dans contentType et check si le type est bien un media type possible //TODO
	private:
		std::map<std::string, std::string> mime;

};
// std::string MediaType::getMediaExtension(std::string requestedType)
// bool	MediaType::isExtensionSupported(std::string	requestedExtension)
// bool	MediaType::isTypeSupported(std::string	requestedType)
// bool 		MediaType::isInAccepted(std::string accepted, std::string fileFormat)
// std::string MediaType::getMediaType(std::string requestedExtension)
#endif /* ******************************************************* MEDIATYPE_H */