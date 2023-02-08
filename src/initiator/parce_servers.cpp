#include <initiator.hpp>

static int	is_value_symbol(const char c)
{
	if ((c > 40 && c < 91) || (c > 94 && c <= 122))
		return (1);
	return (0);
}

std::string	ServerInit::aquire_value(std::string const &server_info_text, const int key_len, const int index) EXCEPTION
{
	std::size_t	iterator = index + key_len;
	std::size_t	begin = 0;
	std::size_t	end = 0;

	while (server_info_text[iterator] != ';' && server_info_text[iterator])
	{
		if (!begin && is_value_symbol(server_info_text[iterator]))
			begin = iterator;
		if (begin)
		{
			if (end && is_value_symbol(server_info_text[iterator]))
				throw SemiCollonFailure();
			if ((!end && !is_value_symbol(server_info_text[iterator]))) {
				end = iterator;
			}
		}
		iterator++;
	}
	if (!end && server_info_text[iterator] == ';')
		end = iterator;
	return (server_info_text.substr(begin, end - begin));
}

template <typename NoKeyExcept, typename ValueExcept>
std::list<std::string>	ServerInit::get_list_of_values_by_word(std::string const &server_info_text,
							const char *key_word, int (*ValueTest)(std::string const&)) EXCEPTION
{
	std::list<std::string>	list_of_ports;
	int						occurance_iterator = 1;

	while (find_word(server_info_text, key_word, occurance_iterator) <= server_info_text.length() - strlen(key_word))
	{
		list_of_ports.push_back(get_value_by_keyword<NoKeyExcept, ValueExcept>(server_info_text,
									key_word, occurance_iterator, ValueTest));
		occurance_iterator++;
	}
	if (occurance_iterator == 1)
		throw NoKeyExcept();
	return (list_of_ports);
}

template <typename NoKeyExcept, typename ValueExcept>
std::string	ServerInit::get_value_by_keyword(std::string const &server_info_text,
				const char *key_word, const int occurance, int (*ValueTest)(std::string const&)) EXCEPTION
{
	std::size_t	value_position;

	value_position = find_word(server_info_text, key_word, occurance);
	if (value_position >= server_info_text.length() - strlen(key_word))
		throw NoKeyExcept();
	if ((*ValueTest)(aquire_value(server_info_text, strlen(key_word), value_position)))
		throw ValueExcept();
	return (aquire_value(server_info_text, strlen(key_word), value_position).c_str());
}

int ServerInit::pathcheck(std::string const &path)
{
	return (access(path.c_str(), R_OK | W_OK));
}

int ServerInit::isPositiveNumber(std::string const &str)
{
	std::stringstream ss(str);
	double num;

	if (ss >> num && num > 0 && ss.eof())
		return false;
	else
		return true;
}

int ServerInit::validIndexFile(std::string const &str)
{
	(void) str;
	return (false);
}

int ServerInit::validServerName(std::string const &str)
{
	(void) str;
	return (false);
}

void	ServerInit::parce_server(std::string const &server_info)
{
	server_info_t	info;

	info.port_number = get_list_of_values_by_word<NoPort, NotNumber>(server_info, "listen", isPositiveNumber);
	info.root_dir = get_value_by_keyword<NoRoot, InvalidRoot>(server_info, "root", 1, pathcheck);
	info.index = get_value_by_keyword<NoIndex, WrongIndex>(server_info, "index", 1, validIndexFile);
	info.server_name = get_value_by_keyword<NoServerName, InvalidServerName>(server_info, "server_name", 1, validServerName);
	// info.server_name = get_value_by_keyword<NoServerName, InvalidServerName>(server_info, "server_name", 1, validServerName);
	// info.host = 
	// info.host = 
	// info.root_dir.append("/home/websrv/http");

	servers_info.push_back(info);
}

void	ServerInit::parce_servers(std::list<std::string> const &server_definitions)
{
	std::list<std::string>::const_iterator	iterator = server_definitions.begin();

	while (iterator != server_definitions.end())
	{
		parce_server(*iterator);
		iterator++;
	}
}