#include <HTTP_Server.hpp>

/*
	Itterates trough socket addr_list, with each address list attempts to:
	1. initate socket https://man7.org/linux/man-pages/man2/socket.2.html
	2. bind to socket https://man7.org/linux/man-pages/man2/bind.2.html
*/

static void set_socket_as_non_blocking(const int socket_fd)
{
	int flags;
	
	flags = fcntl(socket_fd, F_GETFL, 0);
	if (flags == -1) {
		// handle error getting socket flags
		std::cerr << "can not set socket as non-blocking" << std::endl;
		exit (0);
	}

	flags |= O_NONBLOCK;
	if (fcntl(socket_fd, F_SETFL, flags) == -1) {
		// handle error setting socket flags
		std::cerr << "can not set socket as non-blocking" << std::endl;
		exit (0);
	}
}

int	HTTP_Server::init_socket_from_address_list(struct addrinfo *socket_addr_list) EXCEPTION
{
	int	socket_fd;
	int optval = 1;
	const struct addrinfo *socket_addr_list_node;

	socket_addr_list_node = socket_addr_list;
	while (socket_addr_list_node != LIST_END)
	{
		socket_fd = socket(socket_addr_list_node->ai_family,
							socket_addr_list_node->ai_socktype, socket_addr_list_node->ai_protocol);
		if (socket_fd != FAILED_SOCKET) {
			setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
			if (bind(socket_fd, socket_addr_list_node->ai_addr, socket_addr_list_node->ai_addrlen) == SUCESS) {
				set_socket_as_non_blocking(socket_fd);
				break;
			}
			close(socket_fd);
		}
		socket_addr_list_node = socket_addr_list_node->ai_next;
	}
	freeaddrinfo(socket_addr_list);
    if (socket_addr_list_node == NULL)
		throw BindFailure();
	return (socket_fd);
}

/*
	getaddrinfo function wrapper
	https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/
static struct addrinfo *get_socket_address_info_list(const struct addrinfo *TCP_service_info, char const *port_number)
{
	struct addrinfo *result_addr_list;
	int				gai_error_code;

	gai_error_code = getaddrinfo(NULL, port_number, TCP_service_info, &result_addr_list);
	if (gai_error_code != SUCESS) {
		perror("getaddrinfo:\n");
		perror(gai_strerror(gai_error_code));
        exit(EXIT_FAILURE);
	}
	return (result_addr_list);
}

static void	set_addrinfo_for_http(struct addrinfo *TCP_service_info)
{
	memset(TCP_service_info, 0, sizeof(struct addrinfo));
	TCP_service_info->ai_family = AF_INET;
	TCP_service_info->ai_socktype = SOCK_STREAM;
	TCP_service_info->ai_flags = AI_PASSIVE;
	TCP_service_info->ai_protocol = 0;
}

int HTTP_Server::bind_kernel_socket(const struct addrinfo *desired_TCP_service_info, char const *port_number)
{
	struct addrinfo *possible_socket_addr_list;

	possible_socket_addr_list = get_socket_address_info_list(desired_TCP_service_info, port_number);
	try
	{
		return (init_socket_from_address_list(possible_socket_addr_list));
	}
	catch (...)
	{
		std::cerr << "cannot bind" << std::endl;
		exit(EXIT_FAILURE);
	}
}

// static int	learn_mtu_size(const int sockfd)
// {
// 	int mtu = 0;
// 	socklen_t mtu_len = sizeof(mtu);
// 	if (getsockopt(sockfd, SOL_SOCKET, SO_MAX_MSG_SIZE, &mtu, &mtu_len) < 0) {
// 		std::cerr << "Failed to get MTU" << std::endl;
// 		return 1;
// 	}
// 	std::cout << "MTU: " << mtu << std::endl;
// }

#define MAX_REQUEST_QUE 1000000

/*
	Starts socket in kernel, binds to it, and listen. -> returns fd of listening of port
*/
void	HTTP_Server::initiateListeningSocket(char const *port_number) EXCEPTION
{
	struct addrinfo desired_TCP_service_info;

	set_addrinfo_for_http(&desired_TCP_service_info);
	listening_socket_fd.push_back(bind_kernel_socket(&desired_TCP_service_info, port_number));
	if (listen(listening_socket_fd.back(), MAX_REQUEST_QUE) != SUCESS)
		throw BindFailure();
}
