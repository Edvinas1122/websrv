#ifndef SERVEREXECUTION_HPP
# define SERVEREXECUTION_HPP

# include <includes.hpp>
# include <HTTP_Server.hpp>
# include <BroadCastExecutor.hpp>
# include <ObserverAPI.hpp>
# include <initiator.hpp>

# define QUE_IS_NOT_EMPTY(vec) (!vec.empty())
# define STREAM_A_PACKET(que_iter) ((*que_iter)->stream_packet())
# define SOCKETS_LOUD(x) (observer.checkFD(server->server_info().listening_socket_fd[x]))

class HTTP_Server;
class BroadCastExecutor;
class ServerInit;
class ObserverAPI;

class ServerExecution 
{
	public:
		typedef std::list<BroadCastExecutor> clientQueVec;

	private:
		HTTP_Server		*server;
		clientQueVec	client_que;
		ObserverAPI		observer;

	public:
		ServerExecution() {};
		~ServerExecution();

	/*
		Initiator
	*/
		void	parceConfigurationFile(const char *file_path);

	/*
		Controler
	*/
		void	RunTime(void);
		void	Quit(void);

		/* info display */
		void	printInfo(void);
	
	private:
		/* Initialize */
		void	constructServer(ServerInit &initiator);

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
		void	acceptClient(void);
		void	addToBroadCastQue(BroadCastExecutor stream);


		bool	noNewRequests();

	class	RequestTimeOut: public std::exception {};
};

#endif