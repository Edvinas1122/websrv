#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <ServerExecution.hpp>
# include <CGI.hpp>

# define LAST_PACKET false

typedef struct server_info_s server_info_t;
typedef struct Route_s Route;

class VirtualServer
{
	private:
		std::string							host;
		std::string							server_name;
		std::string							root_dir;
		std::string							index;
		std::map<std::string, std::string>	cgi_response;
		std::list<std::string>				port_number;
		std::map<std::string, Route>		routes;

	private:
		std::map<std::string, CGI_Median>	cgi;

	public:
		VirtualServer(server_info_t const &info);
		~VirtualServer(void) {};


		server_info_t	info() const;
		bool			check_for_cgi(std::string const &file_extention);
		int				executeCGI(std::string const &file_extention, std::string const &script, std::string const &query);
		/* END */

	private:
		/* assign CGI - Init */
		void	assignCGI(std::map<std::string, std::string> const &cgi_response);

	class		EpollFailure: public std::exception {};
};

#endif
