#include <CGI.hpp>

int CGI_Median::execute(std::string const &cgi_script_path, std::string const &query) EXCEPTION
{
	pid_t	fork_status;
	int		pipe_fd[2];
	char	*script_path[4];

	script_path[0] = (char *)cgi_executable_path.c_str();
	std::cout << cgi_script_path << std::endl;
	// std::cout << query << std::endl;
	script_path[1] = (char *)"-q";
	script_path[2] = (char *)"/home/git_websrv/http/index.php";
	script_path[3] = NULL;
	(void) query;
	(void) cgi_script_path;

	if (pipe(pipe_fd))
		throw ExecutionFailure();
	fork_status = fork();
	if (fork_status == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);

		execve(script_path[0], script_path, NULL);
		exit(EXIT_FAILURE);
	}
	waitpid(fork_status, NULL, WUNTRACED);
	close(pipe_fd[1]);
	fflush(stdout);
	return(pipe_fd[0]);
}
