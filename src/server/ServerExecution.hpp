#ifndef SERVEREXECUTION_HPP
# define SERVEREXECUTION_HPP

# include <includes.hpp>
# include <VirtualServer.hpp>
# include <BroadCastExecutor.hpp>
# include <ObserverAPI.hpp>
# include <ConfigParcer.hpp>

# define QUE_IS_NOT_EMPTY(vec) (!vec.empty())
# define STREAM_A_PACKET(que_iter) ((*que_iter)->stream_packet())

class HTTP_Server;
class BroadCastExecutor;
class ConfigParcer;
class ObserverAPI;

class ServerExecution 
{
	public:
		typedef std::list<BroadCastExecutor>	clientQueVec;
		typedef std::map<int, std::string>		socketsMap;

	private:
		socketsMap		ServersSocketsFd;
		clientQueVec	client_que;
		ObserverAPI		observer;

	public:
		ServerExecution() {};
		~ServerExecution();

	/*
		Initiator
	*/
		void	initiateServer(const char *file_path);

	/*
		Controler
	*/
		void	RunTime(void);
		void	Quit(void);

		/* info display */
		void	printInfo(void);
	
	private:

	/*
		Sockets
	*/
		void	initiateSockets(std::list<std::string> port_number);
		void	assignObserverToSockets(void);

		/* Socket Bind - Init */
		int	bind_kernel_socket(const struct addrinfo *desired_TCP_service_info, char const *port_number);
		int	init_socket_from_address_list(struct addrinfo *socket_addr_list) EXCEPTION;
		int	initiateListeningSocket(char const *port_number) EXCEPTION;

	/* RunTime */

	/*
		Serveque Que
	*/
		void	serveQue(void);
		bool	clientObservation(clientQueVec &client_que, clientQueVec::iterator &que_iter);
		bool	serveClient(clientQueVec &client_que, clientQueVec::iterator &que_iter);

	/*
		Accept Client
	*/
		void	incomingMonitorAndHandler();
		void	findLoudSockets(std::vector<int> &loudSocketsList);
		void	acceptClient(int loudSocketFd);
		void	addToBroadCastQue(BroadCastExecutor stream);
		bool	testIfAlreadyAccepted(const int client);

		// bool	noNewRequests();

	class	RequestTimeOut: public std::exception {};
	class	BindFailure: public std::exception {};
};

#endif