#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <ServerExecution.hpp>

# include <CGI.hpp>

# define LAST_PACKET false

typedef struct server_info_s server_info_t;
typedef struct Route_s Route;

class HTTP_Server
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
		std::vector<int>					listening_socket_fd;
		std::map<std::string, CGI_Median>	cgi;

	public:
		HTTP_Server(server_info_t &info);
		~HTTP_Server(void) {};

		void			initiateSockets(std::list<std::string> port_number);

		server_info_t	server_info() const;
		bool			check_for_cgi(std::string const &file_extention);
		int				executeCGI(std::string const &file_extention, std::string const &script, std::string const &query);

	private:
		/*initiation*/
		int		bind_kernel_socket(const struct addrinfo *desired_TCP_service_info, char const *port_number);
		int		init_socket_from_address_list(struct addrinfo *socket_addr_list) EXCEPTION;
		void	initiateListeningSocket(char const *port_number) EXCEPTION;
		void	assignCGI(std::map<std::string, std::string> const &cgi_response);

	class		EpollFailure: public std::exception {};
	class		BindFailure: public std::exception {};
};

#endif
