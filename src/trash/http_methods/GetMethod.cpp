#include <GetMethod.hpp>

// static int	buffer_all_file(const int fd, std::string &file)
// {
// 	char	data_buffer[BYTES_PER_PACKET + 1];
// 	int		bytes_read;

// 	bytes_read = 1;
// 	while (bytes_read)
// 	{
// 		memset(data_buffer, 0, BYTES_PER_PACKET + 1);
// 		bytes_read = read(fd, data_buffer, BYTES_PER_PACKET);
// 		file.append(data_buffer, bytes_read);
// 	}
// 	return (bytes_read);
// }

// bool	GetMethod::stream_packet(void)
// {
// 	std::string	file;

// 	// if (this->first_packet())
// 	send(_user_agent_fd, "HTTP/1.1 200 OK\n\n", 17, 0);
// 	buffer_all_file(_file_to_stream_fd, file);
// 	send(_user_agent_fd, file.c_str(), file.length(), 0);
// 	// this->close_connection();
// 	return (false);
// }

bool	GetMethod::stream_packet(void)
{
	char	data_buffer[BYTES_PER_PACKET];
	int		bytes_read;

	bytes_read = read(_file_to_stream_fd, data_buffer, BYTES_PER_PACKET);
	if (bytes_read > 0)
		send(_user_agent_fd, data_buffer, bytes_read, 0);
	else
		this->close_connection();
	return (IS_LAST(bytes_read));
}
