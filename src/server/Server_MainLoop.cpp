#include <HTTP_Server.hpp>

void	ServerExecution::start(void)
{
	if (client_que.size() < 15)
		this->acceptClient();
	if (QUE_IS_NOT_EMPTY(client_que))
		this->serveQue();
}


void	ServerExecution::end()
{
	close(server->server_info().listening_socket_fd[0]);
}

ServerExecution::~ServerExecution()
{
	clientQueVec::iterator	que_iter;

	close(server->server_info().listening_socket_fd[0]);
	delete server;
}

void	ServerExecution::printInfo(void)
{
	std::cout << "WebServer has started\n";
	std::cout << "Server name: " << server->server_info().server_name << std::endl;
}