#ifndef	BROADCASTEXECUTOR_HPP
# define BROADCASTEXECUTOR_HPP

# include <ServerExecution.hpp>

typedef	struct broadCastInfo_s	broadCastInfo;
class VirtualServer;
class BroadCastExecutor;

class Request
{
	friend	BroadCastExecutor;
	private:
		VirtualServer	*_server;
	private:
		size_t		method;
		size_t		version;
		std::string	path;
		bool		keep_alive;
		std::string	host;
		std::string	query;

	public:
		Request() {_server = NULL;};
		~Request(void) {};

	/*
		Controll
	*/
		void			parce(std::string const &received_message, std::string const &port) EXCEPTION;
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
		std::string	get_host(std::string const &received_message);

		/* Access */
		int	access_path(void);

		/* 
			URL parts extractors
		*/
		std::string	fileExtension() const;
		std::string	file() const;

	private:
		typedef std::map<std::string, size_t>	InfoMap;
		static InfoMap	request_methods;
		static InfoMap	versions;
	
	private:
		static InfoMap	map_methods();
		static InfoMap	map_versions();

	public:
		class	InvalidRequest: public std::exception {};
};

typedef struct server_info_s server_info_t;

typedef	struct broadCastInfo_s {
	int			method;
	int			file_fd;
	bool		keep_alive;
}	broadCastInfo;

class ServerInit;

#define REQUEST_TIMEOUT 4
#define	MAX_HEADER_LEN 10000
# define BUFFER_SIZE 1024

class BroadCastExecutor
{
	private:
		struct timeval	timeoutBegin;
		std::string		message;
		Request			client_request;
		int				client_fd;
		broadCastInfo	info;
		std::string		port;
		bool			informed_client;
		bool			qued_request;
		char			data_buffer[BUFFER_SIZE + 1];

	public:
		BroadCastExecutor(const int client_fd, std::string const &port): client_fd(client_fd), port(port)
		{
			memset(&info, 0, sizeof(broadCastInfo));
			memset(&timeoutBegin, 0, sizeof(struct timeval));
			informed_client = false;
			qued_request = false;
		}
		~BroadCastExecutor();

	/*
		Control interface
	*/
		bool	controller(const bool in, const bool out);
		void	ReceiveRequest() EXCEPTION;
		void	Respond() EXCEPTION;
		bool	ServeRequest();
		void	close_connection();
	/* BroadCast */
	private:
		bool	receiveBufferRead() EXCEPTION;
		bool	TimeoutCountMonitor();
		bool	protectReceive();
		bool	testEnd();
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

	/*
		Adittional
	*/
		static void	printServersInfo();

	public:
		typedef std::list<VirtualServer> VirtualServerList;
	
	private:
		static VirtualServerList	servers;
	
	public:
		static void	SetVirualServers(std::list<server_info_t> const &list);
		static VirtualServer	*GetVirtualServer(std::string const &host, std::string const &port);

	public:
		class	RequestFail: public std::exception {};
			class	RequestTimeOut: public RequestFail {};
			class	BadRequest: public RequestFail {};
				class	ReceiveFailure: public BadRequest {};
		class	BroadCastFailure: public std::exception {};
			class	SendingFailure: public BroadCastFailure {};
};

#endif