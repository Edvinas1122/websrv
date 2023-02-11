// static const char	*default_config_path(const char *config_file_path)
// {
// 	char		*cwd = NULL;
// 	std::string	path;

// 	cwd = getcwd(cwd, 0);
// 	path = cwd;
// 	path += "/";
// 	path += config_file_path;
// 	free(cwd);
// 	return (path.c_str());
// }


/*
	for each { symbol after the word "server", there is a matching } symbol
*/
// static int	checkEnclosure(std::string const &configurations)
// {
// 	std::size_t server_pos = configurations.find("server");
// 	std::size_t brace_count = 0;
// 	std::size_t close_brace_count = 0;
	
// 	while (server_pos != std::string::npos)
// 	{
// 		server_pos = configurations.find("{", server_pos);
// 		if (server_pos != std::string::npos) {
// 			brace_count++;
// 			server_pos++;
// 		}
// 	}
	
// 	server_pos = configurations.find("}");
	
// 	while (server_pos != std::string::npos) {
// 		close_brace_count++;
// 		server_pos = configurations.find("}", server_pos + 1);
// 	}
	
// 	if (brace_count == close_brace_count)
// 		return (0);
// 	return (-1);
// }