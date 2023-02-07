#ifndef GET_METHOD_HPP
# define GET_METHOD_HPP

# include <BroadCast.hpp>

class GetMethod: public BroadCast
{
	public:
		const int	_file_to_stream_fd;
	public:
		GetMethod(const int user_agent_fd, const int file_to_stream_fd): 
						BroadCast(user_agent_fd), _file_to_stream_fd(file_to_stream_fd) {};
		virtual ~GetMethod(void) {close(_file_to_stream_fd);};

		virtual bool	stream_packet(void);
};

#endif
