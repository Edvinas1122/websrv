#include <HTTP_Server.hpp>

ServerExecution::~ServerExecution()
{
	clientQueVec::iterator	que_iter;

	close(server->server_info().listening_socket_fd[0]);
	delete server;
}

/*
	Initiator
*/

void	ServerExecution::constructServer(ServerInit &initiator)
{

	server = new HTTP_Server(initiator.get_server_info(0));
	observer.AssignSocket(server->server_info().listening_socket_fd[0]);
}

void	ServerExecution::parceConfigurationFile(const char *config_file_path)
{
	ServerInit	initiator(config_file_path);

	constructServer(initiator);
}

/*
	Controler
*/

void	ServerExecution::RunTime(void)
{
	if (client_que.size() < 15)
		this->acceptClient();
	if (QUE_IS_NOT_EMPTY(client_que))
		this->serveQue();
}

void	ServerExecution::Quit()
{
	close(server->server_info().listening_socket_fd[0]);
}

/* info display */
void	ServerExecution::printInfo(void)
{
	std::cout << "WebServer has started\n";
	std::cout << "Server name: " << server->server_info().server_name << std::endl;
}