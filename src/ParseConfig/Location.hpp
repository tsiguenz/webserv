#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <list>
# include <map>

class Location {
	public:
		// Object managment
		Location();
		Location(Location const& l);
		virtual ~Location();

		// Accessors
		std::string					getPath() const;
		std::string					getErrorPageByCode(int const& errorCode) const;
		std::map<int, std::string>	getErrorPages() const;
		int							getClientMaxBodySize() const;
		bool						getAutoIndex() const;
		std::string					getRoot() const;
		std::string					getIndex() const;
		std::list<std::string>		getAllowedMethods() const;
		bool						isAllowedMethod(std::string const& method) const;
		std::list<std::string>		getAllowedExtCgi() const;
		bool						isAllowedExtCgi(std::string const& ext) const;
		std::string					getUploadPath() const;
		int							getReturnCode() const;
		std::string					getReturnPath() const;

		void	setPath(std::string const& root);
		void	setErrorPage(int const& errorCode, std::string const& path);
		void	setAutoIndex(bool const& autoIndex);
		void	setRoot(std::string const& root);
		void	setIndex(std::string const& index);
		void	setAllowedMethod(std::string const& method);
		void	setAllowedExtCgi(std::string const& ext);
		void	setUploadPath(std::string const& path);
		void	setReturn(int const& code, std::string const& path);

	private:
		std::string					_path;
		std::map<int, std::string>	_errorPages;
		bool						_autoIndex;
		std::string					_index;
		std::string					_root;
		std::list<std::string>		_allowedMethods;
		std::list<std::string>		_allowedExtCgi;
		std::string					_uploadPath;
		int							_returnCode;
		std::string					_returnPath;
	};

#endif // LOCATION_HPP
