#include <ConfigParcer.hpp>

ConfigParcer::ConfigParcer(const char *config_file_path) EXCEPTION
{
	accessCheck(config_file_path);
	loadFile(config_file_path);
	removeComents();
	basicValidate();
}

/*
	Parsers
*/

std::list<server_info_t>	ConfigParcer::getVirtualServersList()
{
	std::list<std::string>		servers_definitions;
	std::list<server_info_t>	VirtualServerList;

	extractVSDefinitionsList(servers_definitions);
	VirtualServerList = parseServers(servers_definitions); //in parce_servers.cpp
	return (VirtualServerList);
}

/*
	Load File In
*/
void	ConfigParcer::accessCheck(const char *config_file_path) EXCEPTION
{
	if (access(config_file_path, R_OK))
		throw ConfFileAccessFailure();
}

void	ConfigParcer::loadFile(const char *config_file_path) EXCEPTION
{
	std::ifstream file(config_file_path);

	configFileContent = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

	/* from string object remove in lines content after # symbol */
void	ConfigParcer::removeComents()
{
	std::stringstream	stream(configFileContent);
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
	configFileContent = result;
}

/*
	Validation - in basic_validation.cpp
*/


/*
	Extract server definitions from configfileContent
*/
template <typename container>
void	ConfigParcer::extractVSDefinitionsList(container &servers_definitions)
{
	int	iterator = 1;
	while (extractVSDefinition(configFileContent, "server", iterator) != "No Occurance")
	{
		servers_definitions.push_back(extractVSDefinition(configFileContent, "server", iterator));
		iterator++;
	}
}

std::string ConfigParcer::extractVSDefinition(std::string const &content,
												const char *definition, const int occurance) EXCEPTION
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

int	ConfigParcer::check_allowed_neighbours(char c)
{
	if (c == ' ' || c == ';' || c == '{' || c == '\n' || c == '\t')
		return (HAS_ALLOWED);
	return (-1);
}

int	ConfigParcer::check_match_word(std::string const &content, std::size_t index, std::size_t word_len)
{
	if (index != 0){
		if (check_allowed_neighbours(content[index - 1]) != HAS_ALLOWED)
			return (-1);
	}
	if (check_allowed_neighbours(content[index + word_len]) != HAS_ALLOWED)
		return (-1);
	return (MATCH);
}

std::size_t	ConfigParcer::find_word(std::string const &content, const char *word_to_match, const std::size_t wanted_occurance)
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

std::size_t	ConfigParcer::find_braket_end(std::string const &content, const std::size_t start_at)
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

// server_info_t	&ConfigParcer::get_server_info(int index)
// {
// 	std::list<server_info_t>::iterator iterator = servers_info.begin();

// 	while (index--)
// 	{
// 		iterator++;
// 	}
// 	if (iterator == servers_info.end())
// 		throw NoServerAtIndex();
// 	return (*iterator);
// }
