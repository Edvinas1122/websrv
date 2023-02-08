#ifndef CGI_MEDIAN_HPP
# define CGI_MEDIAN_HPP

#include <includes.hpp>

class CGI_Median
{
	private:
		std::string	cgi_executable_path;

	public:
		CGI_Median(void) {};
		CGI_Median(std::string const &cgi_executable_path):
						cgi_executable_path(cgi_executable_path) {};
		CGI_Median(const CGI_Median &src): cgi_executable_path(src.cgi_executable_path) {};
		CGI_Median	&operator=(const CGI_Median &src) {
			cgi_executable_path.append(src.cgi_executable_path);
			return *this;
		};
		~CGI_Median() {};
		int	execute(std::string	const &cgi_script_path, std::string const &query) EXCEPTION;
	
	class	ExecutionFailure: public std::exception {};
};

#endif
