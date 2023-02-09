#ifndef	BROADCASTEXECUTOR_HPP
# define BROADCASTEXECUTOR_HPP

# include <ServerExecution.hpp>

typedef	struct broadCastInfo_s	broadCastInfo;
class HTTP_Server;
class BroadCastExecutor;

class Request
{
	private:
		HTTP_Server	*_server;
	private:
		size_t		method;
		size_t		version;
		std::string	path;
		bool		keep_alive;

	public:
		Request(HTTP_Server *server): _server(server) {};
		~Request(void) {};

	/*

	*/
		void			parce(std::string const &received_message) EXCEPTION;
		broadCastInfo	getBroatCastInfo();
		int				Validate();

	private:
	/*
		Parsing
	*/
		void	validate_received_message(std::string const &received_message);
		void	parceRequestLine(std::string const &received_message) EXCEPTION;

		/* Request Line string extractors */
		size_t	determine_method(std::string const &received_message);
		size_t	determine_version(std::string const &received_message);
		std::string	determine_path(std::string const &received_message);

		/* Access */
		int	access_path(void);

		/* 
			URL parts extractors
		*/
		std::string	fileExtension() const;
		std::string	file() const;

	private:
		typedef std::map<std::string, size_t>	InfoMap;
		static std::map<std::string, size_t>	request_methods;
		static std::map<std::string, size_t>	versions;
	
	private:
		static InfoMap	map_methods();
		static InfoMap	map_versions();

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
		Request	client_request;
		int				client_fd;
		broadCastInfo	info;
	public:
		BroadCastExecutor(HTTP_Server *context, const int client_fd):
							client_request(context), client_fd(client_fd)
		{
			memset(&info, 0, sizeof(broadCastInfo));
		}
		~BroadCastExecutor();

	/*
		Control interface
	*/

		void	ReceiveRequest() EXCEPTION;
		void	Respond() EXCEPTION;
		bool	ServeRequest();
		void	close_connection();
	/* BroadCast */
	private:
		void	receive_http_request(std::string &message) EXCEPTION;
		bool	streamPacket() EXCEPTION;
	
	public:
	/*
		Getters
	*/
		int		info_UserFd() const;
		int		info_Method() const;
	/*
		Setters
	*/
		void	set_KeepAlive();


	public :
		class	RequestTimeOut: public std::exception {};
		class	BadRequest: public std::exception {};
			class	ReceiveFailure: public BadRequest {};
		class	BroadCastFailure: public std::exception {};
			class	SendingFailure: public BroadCastFailure {};
};

#endif