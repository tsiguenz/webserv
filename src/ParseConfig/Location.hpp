#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <list>

class Location {
	public:
		// Object managment
		Location();
		virtual ~Location();

		// Accessors
		std::string					getPath() const;
		std::list<std::string>		getAllowedMethods() const;
		std::string					getRedir() const;
		std::string					getRoot() const;
		bool						getAutoIndex() const;
		std::string					getIndex() const;
		std::list<std::string>		getAllowedExtCGI() const;
		std::string					getUploadPath() const;

	private:
		std::string					_path;
		std::list<std::string>		_allowedMethods;
		std::string					_redir;
		std::string					_root;
		bool						_autoIndex;
		std::string					_index;
		std::list<std::string>		_allowedExtCGI;
		std::string					_uploadPath;
	};

#endif // LOCATION_HPP
