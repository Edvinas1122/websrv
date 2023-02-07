#include <ErrorMessage.hpp>

bool	ErrorMessage::stream_packet(void)
{
	char	data_buffer[BYTES_PER_PACKET];
	int		bytes_read;

	if (this->first_packet())
		send(_user_agent_fd, "HTTP/1.0 400 Bad Request\n\n", 26, 0);
	if (_file_to_stream_fd > 0)
	{
		bytes_read = read(_file_to_stream_fd, data_buffer, BYTES_PER_PACKET);
		if (bytes_read > 0)
			write(_user_agent_fd, data_buffer, bytes_read);
		else
			this->close_connection();
		return (IS_LAST(bytes_read));
	}
	else
	{
		this->close_connection();
		return (false);
	}
}