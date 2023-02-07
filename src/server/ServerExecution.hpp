#ifndef SERVEREXECUTION_HPP
# define SERVEREXECUTION_HPP

# include <includes.hpp>
# include <BroadCastExecutor.hpp>
# include <HTTP_Server.hpp>
# include <ObserverAPI.hpp>
# include <initiator.hpp>

# define QUE_IS_NOT_EMPTY(vec) (!vec.empty())
# define STREAM_A_PACKET(que_iter) ((*que_iter)->stream_packet())
# define SOCKETS_LOUD(x) (observer.checkFD(server->server_info().listening_socket_fd[x]))

typedef bool (*HttpTaskFuncPtr)();

class HTTP_Request;
class BroadCastExecutor;
class HTTP_Server;
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

	private:
		size_t	load_track;

	public:
		ServerExecution() {load_track = 0;};
		~ServerExecution();

		void	start(void);
		void	parceConfigurationFile(const char *file_path);
		void	end(void);
		void	printInfo(void);
	
	private:
		/* Initialize */
		void	constructServer(ServerInit &initiator);
		/* RunTime */
		void	serveQue(void);
		void	addToBroadCastQue(BroadCastExecutor stream);
		bool	clientObservation(clientQueVec &client_que, clientQueVec::iterator &que_iter);

		void	acceptClient(void);

		bool	serveClient(clientQueVec &client_que, clientQueVec::iterator &que_iter);
		// void	incommingRequestHandle(const int user_agent_fd);
		// void	receive_http_request(int user_agent_fd, std::string &received_message) EXCEPTION;

		bool	noNewRequests();

	class	RequestTimeOut: public std::exception {};
};

#endif