#include <ConfigParcer.hpp>

static int	is_value_symbol(const char c)
{
	if ((c > 32 && c < 91) || (c > 94 && c <= 122))
		return (1);
	return (0);
}

std::string	ConfigParcer::aquire_value(std::string const &server_info_text, const int key_len, const int index) EXCEPTION
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
std::list<std::string>	ConfigParcer::get_list_of_values_by_word(std::string const &server_info_text,
							const char *key_word, bool (*ValueTest)(std::string const&)) EXCEPTION
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
std::string	ConfigParcer::get_value_by_keyword(std::string const &server_info_text,
				const char *key_word, const int occurance, bool (*ValueTest)(std::string const&)) EXCEPTION
{
	std::size_t	value_position;

	value_position = find_word(server_info_text, key_word, occurance);
	if (value_position >= server_info_text.length() - strlen(key_word))
		throw NoKeyExcept();
	if ((*ValueTest)(aquire_value(server_info_text, strlen(key_word), value_position)))
		throw ValueExcept();
	return (aquire_value(server_info_text, strlen(key_word), value_position).c_str());
}

bool ConfigParcer::pathcheck(std::string const &path)
{
	return (access(path.c_str(), R_OK | W_OK));
}

bool ConfigParcer::isPositiveNumber(std::string const &str)
{
	std::stringstream ss(str);
	double num;

	if (ss >> num && num > 0 && ss.eof())
		return false;
	else
		return true;
}

bool ConfigParcer::validIndexFile(std::string const &str)
{
	(void) str;
	return (false);
}

bool ConfigParcer::validServerName(std::string const &str)
{
	(void) str;
	return (false);
}

bool ConfigParcer::validCGI(std::string const &str)
{
	if (str[0] != '.' || str.find('$') == std::string::npos)
		return (true);
	else
		return (false);

}

bool ConfigParcer::validPath(std::string const &str)
{
	if (str[0] != '/' || str.find('$') == std::string::npos)
		return (true);
	else
		return (false);
}

bool ConfigParcer::validEnable(std::string const &str)
{
	if (str != "enable" && str != "disable")
		return (true);
	else
		return (false);
}

std::map<std::string, std::string>	ConfigParcer::parce_cgi(std::string const &server_info) EXCEPTION
{
	std::map<std::string, std::string>	cgi_map;
	std::string							cgi_info;
	std::string							extention;
	int									iterator = 1;

	while (true) 
	{
		try {
			cgi_info = get_value_by_keyword<NoCGI, WrongCGIDescriptionAvailable>(server_info, "use_cgi", iterator, validCGI);
		} catch(const NoCGI& e) {
			break;
		}
		extention = cgi_info.substr(0, cgi_info.find('$'));
		cgi_map[extention] = cgi_info.erase(0, cgi_info.find('$') + 1);
		iterator++;
	}
	return (cgi_map);
}

std::string	ConfigParcer::aquire_route(std::string const &server_info_text, const int index) EXCEPTION
{
	std::size_t	iterator = index;
	std::size_t	begin = 0;
	std::size_t	end = 0;

	while (server_info_text[iterator] != '{' && server_info_text[iterator])
	{
		if (!begin && is_value_symbol(server_info_text[iterator]))
			begin = iterator;
		if (begin)
		{
			if (end && is_value_symbol(server_info_text[iterator]))
				throw EnclosureFailure();
			if ((!end && !is_value_symbol(server_info_text[iterator]))) {
				end = iterator;
			}
		}
		iterator++;
	}
	if (!end && server_info_text[iterator] == '{')
		end = iterator;
	return (server_info_text.substr(begin, end - begin));
}

std::pair<std::string, std::string> ConfigParcer::get_route_definition(std::string const &content, const int occurance) EXCEPTION
{
	std::pair<std::string, std::string>	route_definition_text;
	std::size_t begin_index_of_key;
	std::size_t index_of_closing_bracket;
	const char *definition = "location";

	begin_index_of_key = find_word(content, definition, occurance);
	if (begin_index_of_key >= (content.length() - strlen(definition)))
		throw NoLinks();
	route_definition_text.first = aquire_route(content, begin_index_of_key + strlen(definition));
	begin_index_of_key = begin_index_of_key + route_definition_text.first.length();
	index_of_closing_bracket = find_braket_end(content, begin_index_of_key);
	if (!index_of_closing_bracket)
		throw EnclosureFailure();
	begin_index_of_key = content.find("{", begin_index_of_key) + 1;
	route_definition_text.second = content.substr(begin_index_of_key, index_of_closing_bracket - begin_index_of_key - 1);
	return (route_definition_text);
}

Route	ConfigParcer::parce_route_definition(std::string const &route_definition_text) EXCEPTION
{
	Route	route_definition;

	(void) route_definition_text;
	route_definition.response_dir = get_value_by_keyword<NoLinks, WrongDirLinks>(route_definition_text, "dir_route", 1, pathcheck);
	route_definition.upload_dir = get_value_by_keyword<NoUploadDir, InvalidUploadDir>(route_definition_text, "dir_route", 1, pathcheck);
	if ((get_value_by_keyword<NoDLInfo, InvalidDLInfo>(route_definition_text, "dir_listing", 1, validEnable)) == "enable")
		route_definition.directory_listing_enabled = true;
	else
		route_definition.directory_listing_enabled = false;
	route_definition.redirect = get_value_by_keyword<NoLinks, WrongDirLinks>(route_definition_text, "redirect", 1, validServerName);
	route_definition.redirect = get_value_by_keyword<NoDefaultFile, InvalidDefaultFile>(route_definition_text, "default_file", 1, validServerName);
	return (route_definition);
}

std::map<std::string, Route>	ConfigParcer::routeExtract(std::string const &server_info) EXCEPTION
{
	std::map<std::string, Route>		map;
	std::pair<std::string, std::string>	route_definition_text;
	int									iterator = 1;

	while (true)
	{
		try {
			route_definition_text = get_route_definition(server_info, iterator);

		} catch(NoLinks &e) {
			break;
		}
		iterator++;
		map.insert(std::pair<std::string, Route>(route_definition_text.first, parce_route_definition(route_definition_text.second)));
	}
	return (map);
}

server_info_t	ConfigParcer::parceServer(std::string const &server_info)
{
	server_info_t	info;

	info.routes = routeExtract(server_info);
	info.port_number = get_list_of_values_by_word<NoPort, NotNumber>(server_info, "listen", isPositiveNumber);
	info.root_dir = get_value_by_keyword<NoRoot, InvalidRoot>(server_info, "root", 1, pathcheck);
	info.index = get_value_by_keyword<NoIndex, WrongIndex>(server_info, "index", 1, validIndexFile);
	info.server_name = get_value_by_keyword<NoServerName, InvalidServerName>(server_info, "server_name", 1, validServerName);
	info.host = get_value_by_keyword<NoHost, InvalidHost>(server_info, "host", 1, validServerName);
	info.cgi_response = parce_cgi(server_info);
	return (info);
}

std::list<server_info_t>	ConfigParcer::parseServers(std::list<std::string> const &server_definitions)
{
	std::list<server_info_t>				servers_info;
	std::list<std::string>::const_iterator	iterator = server_definitions.begin();

	while (iterator != server_definitions.end())
	{
		servers_info.push_back(parceServer(*iterator));
		iterator++;
	}
	return (servers_info);
}

std::list<std::string>	ConfigParcer::getAllSocketsList()
{
	std::list<std::string>	list;
	std::string				socket_port;	
	int						iterator = 1;

	while (true)
	{
		try {
			socket_port = get_value_by_keyword<NoPort, InvalidPort>(configFileContent,
													"listen", iterator, isPositiveNumber);
			list.push_back(socket_port);
		} catch (NoPort &e) {
			break;
		}
		iterator++;
	}
	return (list);
}
