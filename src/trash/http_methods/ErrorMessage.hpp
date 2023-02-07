#ifndef ERROR_MESSAGE_HPP
# define ERROR_MESSAGE_HPP

# include <BroadCast.hpp>

class ErrorMessage: public BroadCast
{
	public:
	ErrorMessage(const int user_agent_fd, const int file_to_stream_fd): 
					BroadCast(user_agent_fd, file_to_stream_fd) {};
	virtual ~ErrorMessage(void) {};

	virtual bool	stream_packet(void);
};

#endif