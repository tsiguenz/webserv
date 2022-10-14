#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>

class Location {
	public:
		// Object managment
		Location();
		virtual ~Location();

		// Getters
		std::string					getPath();
		std::vector<std::string>	getAllowedMethods();
		std::string					getRedir();
		std::string					getRoot();
		bool						getAutoIndex();
		std::string					getIndex();
		std::vector<std::string>	getAllowedExtCGI();
		std::string					getUploadPath();

	private:
		std::string					_path;
		std::vector<std::string>	_allowedMethods;
		std::string					_redir;
		std::string					_root;
		bool						_autoIndex;
		std::string					_index;
		std::vector<std::string>	_allowedExtCGI;
		std::string					_uploadPath;
	};

#endif // LOCATION_HPP
