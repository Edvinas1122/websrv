#include <ServerExecution.hpp>

static void	set_non_blocking_terminal_read()
{
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

static void	display_information()
{
	std::cout << "Command list:\n 1. end - ends program" << std::endl;
}

static void display_assitant_info()
{
	std::cout << "Unknown input.\nType \"help\" for command list" << std::endl;
}

static void end_program()
{
	std::cout << "Quiting program" << std::endl;
}

static bool	parce_input(std::string const &user_input)
{
	if (!user_input.compare(0, 4, "help"))
		display_information();
	else if (!user_input.compare(0, 3, "end"))
	{
		end_program();
		return (false);
	}
	else
		display_assitant_info();
	return (true);
}


static bool terminal_interface()
{
	char c;
	ssize_t bytes_read;
	std::string	user_input;

	bytes_read = read(STDIN_FILENO, &c, 1);
	if (bytes_read > 0)
	{
		while (bytes_read > 0)
		{
			user_input += c;
			bytes_read = read(STDIN_FILENO, &c, 1);
			if (errno != EAGAIN)
				break;
		}
		if (user_input.length())
			return (parce_input(user_input));
	}
	return (true);
}

int	main(void)
{
	ServerExecution	engine;

	engine.parceConfigurationFile("/home/git_websrv/server.conf");
	set_non_blocking_terminal_read();
	engine.printInfo();
	while (terminal_interface())
		engine.start();
	engine.end();
	return (0);
}
