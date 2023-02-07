#include <ServerExecution.hpp>

// static bool	peek_if_last_buffer(int user_agent_fd)
// {
// 	char	mesg[RECEIVE_BUFFER_SIZE + 1];
// 	int		bytes_received;

// 	bytes_received = recv(user_agent_fd, mesg, RECEIVE_BUFFER_SIZE + 1, MSG_PEEK);
// 	if (RECEIVE_BUFFER_SIZE >= bytes_received)
// 		return (LAST_BUFFER);
// 	else
// 		return (NOT_LAST_BUFFER);
// }


/*
	Non-blocking listening socket, acceptsClient -> adds client to server's serving que
	https://man7.org/linux/man-pages/man2/accept.2.html
*/
void	ServerExecution::acceptClient()
{
	struct sockaddr_in	clientaddr;
	socklen_t			addrlen = sizeof(clientaddr);
	int 				client = 0;

	if (SOCKETS_LOUD(0)) {
		std::cout << "accept Client" << std::endl;
		// usleep(300000);
		client = accept(server->server_info().listening_socket_fd[0],
						(struct sockaddr *) &clientaddr, &addrlen);
		if (client > 0) {
			observer.setFdAsNotBlocking(client);
			observer.AssingClientAgnet(client, EPOLLIN);
			addToBroadCastQue(BroadCastExecutor(server, client));
		}
	}
}