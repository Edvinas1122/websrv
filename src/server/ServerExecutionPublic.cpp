#include <ServerExecution.hpp>

ServerExecution::~ServerExecution()
{
	Quit();
}

/*
	Initiator
*/
void	ServerExecution::initiateServer(const char *config_file_path)
{
	ConfigParcer			parser(config_file_path);
	socketsMap::iterator	sockets_iterator;

	BroadCastExecutor::SetVirualServers(parser.getVirtualServersList());
	initiateSockets(parser.getAllSocketsList());
	assignObserverToSockets();
}

void	ServerExecution::assignObserverToSockets(void)
{
	socketsMap::iterator	sockets_iterator = ServersSocketsFd.begin();

	while (sockets_iterator != ServersSocketsFd.end())
	{
		observer.setFdAsNotBlocking((*sockets_iterator).first);
		observer.AssignSocket((*sockets_iterator).first);
		sockets_iterator++;
	}
}

/*
	Controler
*/

void	ServerExecution::RunTime(void)
{
	if (client_que.size() < 10)
		this->incomingMonitorAndHandler();
	if (QUE_IS_NOT_EMPTY(client_que))
		this->serveQue();
}

void	ServerExecution::Quit()
{
	socketsMap::reverse_iterator	iterator = ServersSocketsFd.rbegin();

	while (iterator != ServersSocketsFd.rend())
	{
		close(iterator->first);
		iterator++;
	}
	ServersSocketsFd.clear();
}

/* info display */
void	ServerExecution::printInfo(void)
{
	BroadCastExecutor::printServersInfo();
}