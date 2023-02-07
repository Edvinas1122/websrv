#include <BroadCast.hpp>

void	BroadCast::close_connection(void)
{
	// shutdown (_user_agent_fd, SHUT_RDWR);
	close(_user_agent_fd);
}

bool	BroadCast::first_packet(void)
{
	if (is_first_packet == true)
	{
		is_first_packet = false;
		return (true);
	}
	return (is_first_packet);
}

BroadCast::BroadCast(const int user_agent_fd):
									_user_agent_fd(user_agent_fd)
{
	is_first_packet = true;
	timeout_monitor = 0;
}

void	BroadCast::respose_codes(const unsigned int code)
{
	if (code == 200)
		send(_user_agent_fd, "HTTP/1.1 200 OK\n\n", 17, 0);
}

void	BroadCast::respond(const unsigned int code)
{
	respose_codes(code);
}

BroadCast::~BroadCast(void)
{
	close_connection();
}

bool	BroadCast::timeoutInfo()
{
	timeout_monitor++;
	if (timeout_monitor > TIMEOUT_THRESHOLD)
		return (true);
	return (false);
}