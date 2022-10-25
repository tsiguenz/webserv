#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//: mime()
Response::Response( Request  src ){

	if (!src.isParsed)
		code = 500;
	else if (src.badRequest)
		code = 400;
    else if (src.uriTooLong)
        code = 414;
	else {
		code = 200;
		method = src.method;
		url = src.url;
		httpVersion = src.httpVersion;
		fieldLines = src.fieldLines;
		body = src.body;
	}
	buildingResponse();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response(){
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


// std::ostream &			operator<<( std::ostream & o, Response const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

void				Response::buildingResponse(void) {
	
	// if (code == 400) {
		
	// }
	response += getTime();
    if (method == "GET")
	    getFile();

	// TODO checking file accepted format
	response += 
	response += "\r\n";
	response += std::string(file.begin(), file.end());
    if (!body.empty()) {
        
    }
}

void					Response::getFile(void) {

	//en attendant le fichier de config
	std::string pathRepertoire = "/mnt/nfs/homes/aboudjel/Desktop/webserv/html";
	DIR *repertoire = opendir(pathRepertoire.c_str());
	if (!repertoire)
	{
		std::cout << "ERREUR: en attendant le fichier config jai mis le path des fichier dans /mnt/nfs/homes/aboudjel/Desktop/webserv/html faut mettre ta racine a toi dans \n";
		std::cout << "Request::checkingFile!" << std::endl;
	}
	closedir(repertoire);
	//
	if (url == "/") //selon fichier de config
		url = "/index.html";
	std::string path = pathRepertoire + url;
	// FILE* fp = std::fopen(path.c_str(), "r"); //DEFINIR LE PATH PAR LE REPERTOIRE DANS CONFIG !!!!!!!
	std::ifstream file(path.c_str());
    if(file.fail()) {
        code = 404;
		std::cout << "404 mon PETIT REUF\n" << std::endl;
        return ;
    }
	this->file = std::vector<char>((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
	file.close();
	std::cout << "c bon igo lurl mene a qlq chose" << std::endl;
	return ;

}

std::string	Response::getTime(void) {

	std::string line = "Date: ";
 	std::time_t result = std::time(NULL);
    line += std::asctime(std::localtime(&result));
	line.insert(line.find('\n'), " GMT\r");
	return (line);
}

// std::string Response::getServerName(void) {
//     std::string line = "Server: WebServ 1.0 \n\r";
//     return line;
// }
// std::string Response::getLength(void) {
// 	std::string line = "Content-Length: ";
// 	line += std::distance(this->file.begin(), this->file.end());
// 	line += "\r\n";
// }

// std::string Response::getContentType(void) {
// 	std::string line = "Content-Type: ";
// }

/*

** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */