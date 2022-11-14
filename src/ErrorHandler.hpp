#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP

# include <map>
# include <sstream>

class ErrorHandler
{

	public:
		ErrorHandler();
		~ErrorHandler();
		std::string generateErrorHtml(int errorCode) const;
		std::string generateStatusMessage(int errorCode) const;
	private:
		std::string templateError;
		std::map<int , std::string> errorMessage;
		std::map<int , std::string> statusCodes;

};


#endif /* **************************************************** ERRORHANDLER_H */
