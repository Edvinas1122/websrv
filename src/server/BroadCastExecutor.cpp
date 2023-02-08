#include <HTTP_Server.hpp>
#include <Request.hpp>
#include <BroadCastExecutor.hpp>


# define NOT_LAST(x) (x > 0 ? true : false)
# define RECEIVE_BUFFER_SIZE 1024
# define BUFFER_SIZE 1024
# define REQUEST_TIMEOUT 100000000000
# define FAILURE 0

BroadCastExecutor::~BroadCastExecutor() {};

/*
	Control interface
*/

void	BroadCastExecutor::ReceiveRequest() EXCEPTION
{
	std::string		message;

	try {
		receive_http_request(message);
	} catch(ReceiveFailure &failMessage) {
		throw BadRequest();
	}
	try {
		client_request.parce(message);
	} catch(Request::InvalidRequest &failMessage) {
		throw BadRequest();
	}
}

void	BroadCastExecutor::Respond() EXCEPTION
{
	int	response_code;
	int	bytes_sent;

	response_code = client_request.Validate();
	if (OK == response_code)
		bytes_sent = send(client_fd, "HTTP/1.1 200 OK\n\n", 17, 0);
	else
		bytes_sent = send(client_fd, "HTTP/1.1 400 Bad Request\n\n", 26, 0);
	if (bytes_sent <= FAILURE)
		throw SendingFailure();
	memset(&info, 0, sizeof(broadCastInfo));
	info = client_request.getBroatCastInfo();
}

bool	BroadCastExecutor::ServeRequest()
{
	bool	last = false;
	if (info.method == GET_METHOD)
		last = streamPacket();

	if (info.keep_alive && last)
		return (true);
	return (last);
}

void	BroadCastExecutor::close_connection()
{
	// shutdown (_user_agent_fd, SHUT_RDWR);
	if (info.file_fd)
		close (info.file_fd);
	close(client_fd);
}

/*
	BroadCast
	responds to socket connection
	1. recv https://man7.org/linux/man-pages/man2/recv.2.html
	2. send https://man7.org/linux/man-pages/man2/send.2.html
*/

void	BroadCastExecutor::receive_http_request(std::string &message) EXCEPTION
{
	char		mesg[RECEIVE_BUFFER_SIZE + 1];

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	if (recv(client_fd, mesg, RECEIVE_BUFFER_SIZE, 0) <= FAILURE)
		throw ReceiveFailure();
	message.append(mesg);
	std::cout << message << std::endl;
}

bool	BroadCastExecutor::streamPacket(void) EXCEPTION
{
	char	data_buffer[BUFFER_SIZE];
	int		bytes_read;
	int		bytes_sent;

	bytes_read = read(info.file_fd, data_buffer, BUFFER_SIZE);
	if (bytes_read > 0)
		bytes_sent = send(client_fd, data_buffer, bytes_read, 0);
	if (bytes_sent <= FAILURE)
		throw SendingFailure();
	return (NOT_LAST(bytes_read));
}


/*
	Getters
*/

int	BroadCastExecutor::info_Method() const
{
	return (info.method);
}

int	BroadCastExecutor::info_UserFd() const
{
	return (client_fd);
}


/*
	Setters
*/

void	BroadCastExecutor::set_KeepAlive()
{
	info.keep_alive = true;
}
