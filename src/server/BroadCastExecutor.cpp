#include <VirtualServer.hpp>
#include <Request.hpp>
#include <BroadCastExecutor.hpp>


# define NOT_LAST(x) (x > 0 ? true : false)
# define RECEIVE_BUFFER_SIZE 1024
# define FAILURE 0

BroadCastExecutor::VirtualServerList	BroadCastExecutor::servers;

BroadCastExecutor::~BroadCastExecutor() {

};

/*
	Control interface
*/

bool	BroadCastExecutor::controller(const bool in, const bool out)
{
	bool	end;

	if (client_request._server == NULL) {
		if (in) {
			try {
				std::cout << "Receiving first request " << std::endl;
				ReceiveRequest();
			} catch (RequestFail &e) {
				return (false);
			} 
		}
		return (true);
	}
	if (in) {
		try {
			std::cout << "Receiving interuptive request " << std::endl;
			ReceiveRequest();
		} catch (RequestFail &e) {
			return (false);
		}
		qued_request = false;
		return (false);
	} else if (out && !informed_client) {
		std::cout << "Responding to a request " << std::endl;
		Respond();
		return (true);
	} else if (out && informed_client) {
		end = ServeRequest();
		if (!end && !qued_request) {
			std::cout << "Serving " << std::endl;
			informed_client = false;
			return (false);
		}
		if (!end && qued_request) {
			std::cout << "Responding qued request " << std::endl;
			// Respond();

			return (false);
		}
	}
	return (true);
}


bool	BroadCastExecutor::testEnd()
{
	if (message.find("\r\n\r\n") != std::string::npos)
		return (true);
	else
		return (false);
}

bool	BroadCastExecutor::protectReceive()
{
	if (message.length() > MAX_HEADER_LEN)
		return (true);
	return (false);
}

bool	BroadCastExecutor::TimeoutCountMonitor()
{
	struct timeval	currentTime;

	if (timeoutBegin.tv_sec == 0)
	{
		gettimeofday(&timeoutBegin, NULL);
		return (false);
	}
	gettimeofday(&currentTime, NULL);
	if (currentTime.tv_sec - timeoutBegin.tv_sec > REQUEST_TIMEOUT)
	{
		std::cout << "request timeout" << std::endl;
		return (true);
	}
	return (false);
}

void	BroadCastExecutor::ReceiveRequest() EXCEPTION
{
	bool	received_buf;

	try {
		received_buf = receiveBufferRead();
	} catch (ReceiveFailure &e) {
		received_buf = false;
	}
	if (received_buf)
	{
		if (testEnd())
		{
			client_request.parce(message, port);
			message.clear();
			memset(&timeoutBegin, 0, sizeof(struct timeval));
		}
		else {
			if (protectReceive())
				throw  BadRequest();
		}
		memset(&timeoutBegin, 0, sizeof(struct timeval));
	}
	else
	{
		if (testEnd())
		{
			memset(&timeoutBegin, 0, sizeof(struct timeval));
			client_request.parce(message, port);
			return;
		}
		if (TimeoutCountMonitor())
			throw RequestTimeOut();
	}
}

/*
	BroadCast
	responds to socket connection
	1. recv https://man7.org/linux/man-pages/man2/recv.2.html
	2. send https://man7.org/linux/man-pages/man2/send.2.html
*/
bool	BroadCastExecutor::receiveBufferRead() EXCEPTION
{
	char	mesg[RECEIVE_BUFFER_SIZE + 1];
	int		bytes_read = 0;

	memset((void*)mesg, (int)'\0', RECEIVE_BUFFER_SIZE + 1);
	bytes_read = recv(client_fd, mesg, RECEIVE_BUFFER_SIZE, 0);
	if (bytes_read > 0) {
		std::cout << message << std::endl;
		message.append(mesg);
		return (true);
	}
	else if (bytes_read == 0)
		return (false);
	throw ReceiveFailure();
}

void	BroadCastExecutor::Respond() EXCEPTION
{
	int	bytes_sent;

	client_request.Validate();
	if (true) {
		bytes_sent = send(client_fd, "HTTP/1.1 200 OK\n\n", 17, 0);
	}
	else
		bytes_sent = send(client_fd, "HTTP/1.1 400 Bad Request\n\n", 26, 0);
	if (bytes_sent > 0)
	{
		info = client_request.getBroatCastInfo();
		informed_client = true;
	}
	if (bytes_sent <= FAILURE)
		throw SendingFailure();
}

bool	BroadCastExecutor::ServeRequest()
{
	bool	last = false;
	if (info.method == GET_METHOD)
		last = streamPacket();
	return (last);
}

void	BroadCastExecutor::close_connection()
{
	// shutdown (_user_agent_fd, SHUT_RDWR);
	if (info.file_fd)
		close (info.file_fd);
	close(client_fd);
}

bool	BroadCastExecutor::streamPacket(void) EXCEPTION
{
	int		bytes_read;
	int		bytes_sent;

	if (data_buffer[0] == 0)
		bytes_read = read(info.file_fd, data_buffer, BUFFER_SIZE);
	else 
		bytes_read = strlen(data_buffer); // there were last buffer from failed send
	if (bytes_read > 0)
		bytes_sent = send(client_fd, data_buffer, bytes_read, 0);
	if (bytes_read > 0 && bytes_sent == bytes_read)
	{
		memset(data_buffer, 0, BUFFER_SIZE + 1);
		return (true);
	}
	if (bytes_read <= 0)
		return (false); // end last
	if (bytes_sent == -1)
		throw SendingFailure();
	if (bytes_sent != bytes_read && bytes_read != 0) // not full buffer were sent, case
	{
		memmove(data_buffer, &data_buffer[bytes_sent], BUFFER_SIZE - bytes_sent);
		memset(&data_buffer[BUFFER_SIZE - (BUFFER_SIZE - bytes_sent)], 0, BUFFER_SIZE - bytes_sent);
		return (true);
	}
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

void	BroadCastExecutor::printServersInfo()
{
	VirtualServerList::iterator			iterator = servers.begin();
	server_info_t						info;
	std::list<std::string>::iterator	attribute_iterator;
	std::map<std::string, std::string>::iterator	map_iterator;
	std::map<std::string, Route>::iterator	route_iterator;
	int									serverCount = 1;

	while (iterator != servers.end())
	{
		int	portCount = 1;

		info = (*iterator).info();
		std::cout << "======= Server Nr." << serverCount++ << "Info ========" << std::endl;
		std::cout << "Server name: " << info.server_name << std::endl;
		std::cout << "Server host: " << info.host << std::endl;
		std::cout << "Available on Ports: " << std::endl;
		attribute_iterator = info.port_number.begin();
		while (attribute_iterator != info.port_number.end())
		{
			std::cout << "  - Port nr." << portCount++ << " :" << (*attribute_iterator) << std::endl;
			attribute_iterator++;
		}
		std::cout << "CGI Responses: " << std::endl;
		map_iterator = info.cgi_response.begin();
		while (map_iterator != info.cgi_response.end())
		{
			std::cout << "  - File extension: " << (*map_iterator).first << " ---> " << (*map_iterator).second << std::endl;
			map_iterator++;
		}
		std::cout << "Server Routes: " << std::endl;
		route_iterator = info.routes.begin();
		while (route_iterator != info.routes.end())
		{
			std::cout << "  - Route: " << (*route_iterator).first << std::endl;
			std::cout << "    - Direct to directory:" << (*route_iterator).second.response_dir << std::endl;
			std::cout << "    - Redirect to :" << (*route_iterator).second.redirect << std::endl;
			route_iterator++;
		}
		iterator++;
	}
}

void	BroadCastExecutor::SetVirualServers(std::list<server_info_t> const &list)
{
	std::list<server_info_t>::const_iterator	iterator = list.begin();

	while (iterator != list.end())
	{
		servers.push_back(VirtualServer(*iterator));
		iterator++;
	}
}



VirtualServer	*BroadCastExecutor::GetVirtualServer(std::string const &host, std::string const &port)
{
	VirtualServerList::iterator	iterator = servers.begin();
	server_info_t	info;

	while (iterator != servers.end())
	{
		info = iterator->info();
		if (info.host == host && std::find(info.port_number.begin(), info.port_number.end(), port) != info.port_number.end())
		{
			std::cout << "found virtual server" << std::endl;
			return (&*iterator);
		}
		iterator++;
	}
	return (&*servers.begin());
}