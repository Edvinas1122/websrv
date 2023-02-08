#ifndef	BROADCASTEXECUTOR_HPP
# define BROADCASTEXECUTOR_HPP

// # include <includes.hpp>
# include <ServerExecution.hpp>

// #include <HTTP_Server.hpp>

class HTTP_Server;
// class BroadCastExecutor;
typedef	struct broadCastInfo_s	broadCastInfo;

class HTTP_Request
{
	private:
		HTTP_Server	*_server;
	private:
		size_t		method;
		size_t		version;
		std::string	requested_by_host;
		std::string	query;
		bool		keep_alive;
		/* Connection: keep-alive */
		/* Cache-Control: max-age=0 */
		/* Upgrade-Insecure-Requests: 1 */
		/* User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36 */
		// std::string	user_agent;
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
		// Accept-Encoding: gzip, deflate
		// Accept-Language: en-US,en;q=0.9

	public:
		HTTP_Request(HTTP_Server *server): _server(server) {};
		// HTTP_Request(HTTP_Request const &src);
		// HTTP_Request &operator=(const HTTP_Request &src);
		~HTTP_Request(void) {};

		void			parce(std::string const &received_message);
		broadCastInfo	getBroatCastInfo();
		int				Validate();

	private:
		/*Parsing*/
		void	validate_received_message(std::string const &received_message);
		void	find_method_version_host_query(std::string const &received_message) EXCEPTION;
		size_t	determine_method(std::string const &received_message);
		size_t	determine_version(std::string const &received_message);
		std::string	determine_query(std::string const &received_message);

		/* Access */
		int		access_path(void);
		std::string	fileExtension() const;
		std::string	file() const;

	public:
		class	InvalidRequest: public std::exception {};
};

typedef	struct broadCastInfo_s {
	int			method;
	int			file_fd;
	bool		not_first_packet;
	bool		keep_alive;
}	broadCastInfo;

class BroadCastExecutor
{
	private:
		HTTP_Request	client_request;
		int				client_fd;
		broadCastInfo	info;
	public:
		BroadCastExecutor(HTTP_Server *context, const int client_fd):
							client_request(context), client_fd(client_fd)
		{
			memset(&info, 0, sizeof(broadCastInfo));
		}
		~BroadCastExecutor();

		void	ReceiveRequest() EXCEPTION;
		void	SetLastReceivedInfo();
		void	Respond() EXCEPTION;
		bool	ServeRequest();
		void	close_connection();

	/* Getters */
		int		user_agent_fd() const;
		int		getMethod() const;
		void	enableKeepAlive();

	private:
		void	receive_http_request(std::string &message) EXCEPTION;
		bool	streamPacket() EXCEPTION;

	public :
		class	RequestTimeOut: public std::exception {};
		class	BadRequest: public std::exception {};
			class	ReceiveFailure: public BadRequest {};
		class	BroadCastFailure: public std::exception {};
			class	SendingFailure: public BroadCastFailure {};
};

#endif