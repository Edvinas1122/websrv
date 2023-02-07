#include <initiator.hpp>
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

void	ServerInit::accessCheck(const char *config_file_path) EXCEPTION
{
	if (access(config_file_path, R_OK))
		throw ConfFileAccessFailure();
}


/*
	from string object remove in lines content after # symbol
*/
static void	removeComents(std::string &configurations)
{
	std::stringstream	stream(configurations);
	std::string			line;
	std::string			result;
	std::size_t			comment_pos;
  
	while (std::getline(stream, line)) {
		comment_pos = line.find('#');
		if (comment_pos != std::string::npos) {
			line.erase(comment_pos);
		}
		result += line + '\n';
	}
	configurations = result;
}

void	ServerInit::loadFile(const char *config_file_path, std::string &configurations) EXCEPTION
{
	std::ifstream file(config_file_path);

	configurations = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	removeComents(configurations);
}

// static std::string trim_white_spaces(const std::string& str) {
// 	std::size_t first = str.find_first_not_of(' ');
// 	if (first == std::string::npos) {
// 		return "";
// 	}
// 	std::size_t last = str.find_last_not_of(' ');
// 	return str.substr(first, last - first + 1);
// }

#define HAS_ALLOWED 0
#define MATCH 0

int	ServerInit::check_allowed_neighbours(char c)
{
	if (c == ' ' || c == ';' || c == '{' || c == '\n' || c == '\t')
		return (HAS_ALLOWED);
	return (-1);
}

int	ServerInit::check_match_word(std::string const &content, std::size_t index, std::size_t word_len)
{
	if (index != 0){
		if (check_allowed_neighbours(content[index - 1]) != HAS_ALLOWED)
			return (-1);
	}
	if (check_allowed_neighbours(content[index + word_len]) != HAS_ALLOWED)
		return (-1);
	return (MATCH);
}

std::size_t	ServerInit::find_word(std::string const &content, const char *word_to_match, const std::size_t wanted_occurance)
{
	std::size_t test_occur_index = 0;
	std::size_t occurance = 0;

	while (test_occur_index < content.length())
	{
		test_occur_index = content.find(word_to_match, test_occur_index);
		if (test_occur_index == std::string::npos)
			return (content.length() - 1);
		if (check_match_word(content, test_occur_index, strlen(word_to_match)) == MATCH)
			occurance++;
		if (occurance == wanted_occurance)
			return (test_occur_index);
		test_occur_index++;
	}
	return (test_occur_index);
}

std::size_t	ServerInit::find_braket_end(std::string const &content, const std::size_t start_at)
{
	std::size_t	index = 0;
	int			bracket_count = 0;

	index = content.find("{", start_at);
	while (content[index])
	{
		if (content[index] == '{')
			bracket_count++;
		else if (content[index] == '}')
			bracket_count--;
		if (!bracket_count)
			return (index);
		index++;
	}
	return (0);
}

std::string ServerInit::extract_definition(std::string const &content, const char *definition, const int occurance) EXCEPTION
{
	std::size_t first_server_pos;
	std::size_t second_server_pos;

	first_server_pos = find_word(content, definition, occurance);
	if (first_server_pos >= (content.length() - strlen(definition)))
		return ("No Occurance");
	second_server_pos = find_braket_end(content, first_server_pos);
	if (!second_server_pos)
		throw EnclosureFailure();
	first_server_pos = content.find("{", first_server_pos) + 1;
	return (content.substr(first_server_pos, second_server_pos - first_server_pos - 1));
}

template <typename container>
void	ServerInit::get_server_definitions(std::string const &content, container &servers_definitions)
{
	int	iterator = 1;
	while (extract_definition(content, "server", iterator) != "No Occurance")
	{
		servers_definitions.push_back(extract_definition(content, "server", iterator));
		iterator++;
	}
}

ServerInit::ServerInit(const char *config_file_path) EXCEPTION
{
	std::string				configurations;
	std::list<std::string>	servers_definitions;

	accessCheck(config_file_path);
	loadFile(config_file_path, configurations);
	basicValidate(configurations);
	get_server_definitions(configurations, servers_definitions);
	parce_servers(servers_definitions);
}

server_info_t	&ServerInit::get_server_info(int index)
{
	std::list<server_info_t>::iterator iterator = servers_info.begin();

	while (index)
	{
		iterator++;
	}
	return (*iterator);
}

void ServerExecution::constructServer(ServerInit &initiator)
{

	server = new HTTP_Server(initiator.get_server_info(0));
	observer.AssignSocket(server->server_info().listening_socket_fd[0]);
}

void	ServerExecution::parceConfigurationFile(const char *config_file_path)
{
	ServerInit	initiator(config_file_path);

	constructServer(initiator);
}