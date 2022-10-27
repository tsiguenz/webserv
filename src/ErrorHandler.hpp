#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP

# include "webserv.h"

class ErrorHandler
{

	public:
		ErrorHandler();
		~ErrorHandler();
		std::string	generateErrorHtml(int errorCode);
	private:
		std::string templateError;
		std::map<int , std::string> errorMessage;
};


#endif /* **************************************************** ERRORHANDLER_H */