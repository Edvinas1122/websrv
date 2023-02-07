#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

#include <ServerExecution.hpp>

# define LAST_PACKET false

typedef struct server_info_s server_info_t;

class HTTP_Server
{
	private:
		std::string				host;
		std::string				server_name;
		std::string				root_dir;
		std::string				index;
		bool					directory_listing_enabled;
		std::string				upload_dir;
		std::list<std::string>	response_to_dir;
		std::list<std::string>	cgi_response;
		std::list<std::string>	port_number;

	private:
		std::vector<int>		listening_socket_fd;

	public:
		HTTP_Server(server_info_t &info);
		~HTTP_Server(void) {};

		void	initiateSockets(std::list<std::string> port_number);

		server_info_t	server_info() const;

	private:
		/*initiation*/
		int		bind_kernel_socket(const struct addrinfo *desired_TCP_service_info, char const *port_number);
		int		init_socket_from_address_list(struct addrinfo *socket_addr_list) EXCEPTION;
		void	initiateListeningSocket(char const *port_number) EXCEPTION;

	class		EpollFailure: public std::exception {};
	class		BindFailure: public std::exception {};

};

#endif