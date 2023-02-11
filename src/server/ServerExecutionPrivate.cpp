#include <VirtualServer.hpp>

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

/*
	Serveque Que
*/

bool	ServerExecution::serveClient(clientQueVec &client_que, clientQueVec::iterator &que_iter)
{
	(void) client_que;
	return (que_iter->controller(observer.checkFDforInBound(que_iter->info_UserFd()), observer.checkFD(que_iter->info_UserFd())));
}

void	ServerExecution::serveQue(void)
{
	clientQueVec::iterator	que_iter;

	que_iter = client_que.begin();
	while (QUE_IS_NOT_EMPTY(client_que) && que_iter != client_que.end())
	{
		if (clientObservation(client_que, que_iter))
		{
			std::cout << "write: " << observer.checkFD(que_iter->info_UserFd()) \
			<< " read: " << observer.checkFDforInBound(que_iter->info_UserFd()) << std::endl;
			usleep (100000);
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

bool	ServerExecution::clientObservation(clientQueVec &client_que, clientQueVec::iterator &que_iter)
{
	(void) client_que;
	(void) que_iter;
	if (observer.checkFDforInBound((*que_iter).info_UserFd()))
	{
		return (true);
	}
	if (observer.checkFD((*que_iter).info_UserFd()))
	{
		return (true);
	}
	return (false);
}

// bool	ServerExecution::serveClient(clientQueVec &client_que, clientQueVec::iterator &que_iter)
// {
// 	(void) client_que;
// 	if (!(*que_iter).info_Method()) {
// 		if (observer.checkFDforInBound((*que_iter).info_UserFd()))
// 		{
// 			try {
// 				(*que_iter).ReceiveRequest();
// 			} catch(BroadCastExecutor::BadRequest& e) {
// 				std::cerr << "Bad Request Cannot respond" << '\n';
// 				return (LAST_PACKET);
// 			}
// 			std::cout << "Receiving request in fd ID_" << (*que_iter).info_UserFd() << std::endl;
// 		}
// 		else if (observer.checkFD((*que_iter).info_UserFd()))
// 		{
// 			std::cout << "Responding: " << (*que_iter).info_UserFd() << std::endl;
// 			try {
// 				(*que_iter).Respond();
// 			} catch(BroadCastExecutor::BroadCastFailure& e) {
// 				std::cerr << "failed to respond to client" << '\n';
// 				return (LAST_PACKET);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (observer.checkFDforInBound((*que_iter).info_UserFd()))
// 		{
// 			std::cout << "receiver inbound " << std::endl;
// 			try {
// 				(*que_iter).set_KeepAlive();
// 				(*que_iter).ReceiveRequest();
// 			} catch(BroadCastExecutor::BadRequest& e) {
// 				std::cerr << "cannot receive back from keep alive" << '\n';
// 				return (LAST_PACKET);
// 			}
// 		}
// 		else if (observer.checkFD((*que_iter).info_UserFd()))
// 		{
// 			try {
// 				return ((*que_iter).ServeRequest());
// 			} catch(BroadCastExecutor::BroadCastFailure& e) {
// 				std::cerr << "failure in stream" << '\n';
// 				return (LAST_PACKET);
// 			}
// 		}
// 	}
// 	return (true);
// }

/*
	Accept Client
	Non-blocking listening socket, acceptsClient -> adds client to server's serving que
	https://man7.org/linux/man-pages/man2/accept.2.html
*/
static void	reserveOnce(std::vector<int> &container, const int size)
{
	container.reserve(size);
}

void	ServerExecution::incomingMonitorAndHandler() // minimize reallocation cound
{
	std::vector<int>	loudSocketsList;
	reserveOnce(loudSocketsList, ServersSocketsFd.size());
	size_t				iterator = 0;

	findLoudSockets(loudSocketsList);
	while (iterator < ServersSocketsFd.size())
	{
		if (loudSocketsList[iterator])
			acceptClient(loudSocketsList[iterator]);
		iterator++;
	}
}


void	ServerExecution::acceptClient(int loudSocketFd)
{
	struct sockaddr_in				clientaddr;
	static socklen_t				addrlen = sizeof(clientaddr);
	int 							client = 0;

	client = accept(loudSocketFd, (struct sockaddr *) &clientaddr, &addrlen);
	if (client > 0) {
		if (testIfAlreadyAccepted(client))
			return;
		std::cout << "Client accepted: " << client << std::endl;
		observer.setFdAsNotBlocking(client);
		observer.AssingClientAgnet(client, EPOLLIN);
		addToBroadCastQue(BroadCastExecutor(client, ServersSocketsFd[loudSocketFd]));
	} 
}

bool	ServerExecution::testIfAlreadyAccepted(const int client)
{
	clientQueVec::iterator	iterator = client_que.begin();

	while (iterator != client_que.end())
	{
		if (iterator->info_UserFd() == client)
			return (true);
		iterator++;
	}
	return (false);
	
}

void	ServerExecution::addToBroadCastQue(BroadCastExecutor stream)
{
	client_que.push_back(stream);
}

void	ServerExecution::findLoudSockets(std::vector<int> &loudSocketsList)
{
	socketsMap::iterator		iterator = ServersSocketsFd.begin();
	std::vector<int>::iterator	loudSocketsIterator = loudSocketsList.begin();

	while (iterator != ServersSocketsFd.end())
	{
		if (observer.checkFD(iterator->first))
		{
			std::cout << "loud Socket at port: " << iterator->first << std::endl;
			*loudSocketsIterator = iterator->first;
		}
		else
			*loudSocketsIterator = 0;
		iterator++;
		loudSocketsIterator++;
	}
}