#ifndef HTTP_STREAMPLEX_HPP
# define HTTP_STREAMPLEX_HPP

# include <HTTP_Server.hpp>

# define TIMEOUT_THRESHOLD 2

class BroadCast
{
	protected:
		const int	_user_agent_fd;

	private:
		bool	is_first_packet;
		int		client_epoll_fd;

		size_t	timeout_monitor;

	public:
		BroadCast(const int user_agent_fd);
		// HTTP_StreamPlex(const HTTP_StreamPlex &src):
		// 				_file_to_stream_fd(src._file_to_stream_fd), _user_agent_fd(_user_agent_fd) {};
		
		virtual ~BroadCast(void);
		// HTTP_StreamPlex	&operator=(HTTP_StreamPlex const &src) {*this = src; return (*this);};
		bool			check_client(void) EXCEPTION;
		virtual void	respond(const unsigned int code);
		virtual bool	stream_packet(void) = 0;

		bool			timeoutInfo();

		void			close_connection(void);
		int				user_agent_fd(void) const {return (_user_agent_fd);};
	protected:
		bool first_packet(void);
		void respose_codes(const unsigned int code);
};

#endif