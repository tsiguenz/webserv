
#ifndef MEDIATYPE_HPP
# define MEDIATYPE_HPP

# include <iostream>
# include <vector>
# include <map>
# include <algorithm>

class MediaType
{

	public:

		MediaType();
		MediaType(MediaType const& mt);
		MediaType&	operator=(MediaType const& rhs);
		~MediaType();

		// bool 	isInAccepted(std::string requestedTypes, std::string fileFormat); //(recoit la ligne complete de "Accepted: " et la parse (du coup fait les ordre de prio et tout)) et check si ce quon sapprete a renvoyer et dans accepted //TODO
		// bool	isTypeSupported(std::string	requestedType); //pour post 

		std::string getMediaType(std::string requestedExtension);//recoit la ligne dans contentType et check si le type est bien un media type possible //TODO
		std::string getMediaExtension(std::string requestedType);
		bool 		isInAccepted(std::string accepted, std::string fileFormat);
		bool		isTypeSupported(std::string	requestedType);
		bool		isExtensionSupported(std::string requestedExtension);

	private:
		std::map<std::string, std::string> mime;

};


#endif /* ******************************************************* MEDIATYPE_H */
