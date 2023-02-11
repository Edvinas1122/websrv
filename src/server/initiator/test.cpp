#include "ConfigParcer.hpp"

int	main(void)
{
	ConfigParcer				instance("/home/git_websrv/server.conf");
	std::list<server_info_t>	listOfVirtualServers = instance.getVirtualServersList();

	std::cout << (*((++listOfVirtualServers.begin()))).server_name << std::endl;
	std::cout << (*(++listOfVirtualServers.begin())).host << std::endl;
	std::cout << listOfVirtualServers.begin()->routes.begin()->second.response_dir << std::endl;
	std::cout << instance.getAllSocketsList().front() << std::endl;
	return (0);
}
