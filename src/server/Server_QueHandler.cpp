#include <HTTP_Server.hpp>

template <typename vector, typename iterator>
static void	circle_iterate(vector &vec, iterator &iter)
{
	((++iter) == vec.end()) ? (iter = vec.begin()) : (iter);
}

template <typename vector, typename iterator>
static void	finish_stream(vector &client_que, iterator &iter)
{
	client_que.erase(iter);
	iter = client_que.begin();
}

#define	BEFORE_ACCEPT_CLIENT 4

bool	ServerExecution::clientObservation(clientQueVec &client_que, clientQueVec::iterator &que_iter)
{
	(void) client_que;
	(void) que_iter;
	if (observer.checkFDforInBound((*que_iter).user_agent_fd()))
	{
		return (true);
	}
	if (observer.checkFD((*que_iter).user_agent_fd()))
	{
		return (true);
	}
	return (false);
}

# define RECEIVE_BUFFER_SIZE 1024

bool	ServerExecution::serveClient(clientQueVec &client_que, clientQueVec::iterator &que_iter)
{
	(void) client_que;
	if (!(*que_iter).getMethod()) {
		if (observer.checkFDforInBound((*que_iter).user_agent_fd()))
		{
			try {
				(*que_iter).ReceiveRequest();
			} catch(BroadCastExecutor::BadRequest& e) {
				std::cerr << "Bad Request Cannot respond" << '\n';
				return (LAST_PACKET);
			}
			std::cout << "Receiving request in fd ID_" << (*que_iter).user_agent_fd() << std::endl;
		}
		else if (observer.checkFD((*que_iter).user_agent_fd()))
		{
			std::cout << "Responding: " << (*que_iter).user_agent_fd() << std::endl;
			try {
				(*que_iter).Respond();
			} catch(BroadCastExecutor::BroadCastFailure& e) {
				std::cerr << "failed to respond to client" << '\n';
				return (LAST_PACKET);
			}
		}
	}
	else
	{
		if (observer.checkFDforInBound((*que_iter).user_agent_fd()))
		{
			std::cout << "receiver inbound " << std::endl;
			try {
				(*que_iter).enableKeepAlive();
				(*que_iter).ReceiveRequest();
			} catch(BroadCastExecutor::BadRequest& e) {
				std::cerr << "cannot receive back from keep alive" << '\n';
				return (LAST_PACKET);
			}
		}
		else if (observer.checkFD((*que_iter).user_agent_fd()))
		{
			try {
				return ((*que_iter).ServeRequest());
			} catch(BroadCastExecutor::BroadCastFailure& e) {
				std::cerr << "failure in stream" << '\n';
				return (LAST_PACKET);
			}
		}
	}
	return (true);
}

void	ServerExecution::serveQue(void)
{
	clientQueVec::iterator	que_iter;

	que_iter = client_que.begin();
	while (QUE_IS_NOT_EMPTY(client_que) && que_iter != client_que.end())
	{
		if (clientObservation(client_que, que_iter))
		{
			if (serveClient(client_que, que_iter) == LAST_PACKET)
			{
				(*que_iter).close_connection();
				finish_stream(client_que, que_iter);
				if (que_iter == client_que.end())
					break;
			}
		}
		que_iter++;
	}
}

void	ServerExecution::addToBroadCastQue(BroadCastExecutor stream)
{
	client_que.push_back(stream);
}
