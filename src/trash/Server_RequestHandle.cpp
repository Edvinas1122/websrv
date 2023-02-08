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
