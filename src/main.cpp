#include <ServerExecution.hpp>

#ifdef TERMINAL
void	set_non_blocking_terminal_read();
bool	terminal_interface();
#endif


int	main(void)
{
	ServerExecution	engine;

	engine.initiateServer("/home/git_websrv/server.conf");
	engine.printInfo();

#ifdef TERMINAL
	set_non_blocking_terminal_read();
	while (terminal_interface())
#else
	while (true)
#endif
		engine.RunTime();
	engine.Quit();
	return (0);
}
