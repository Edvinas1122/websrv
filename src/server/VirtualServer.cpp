#include <VirtualServer.hpp>

template <typename container>
static void	copy_list_values(container &destination, container const &source)
{
	std::copy(source.begin(), source.end(), std::back_inserter(destination));
}

VirtualServer::VirtualServer(server_info_t const &info)
{
	host = info.host;
	server_name = info.server_name;
	root_dir = info.root_dir;
	index = info.index;
	if (info.cgi_response.size())
		cgi_response = info.cgi_response;
	if (info.port_number.size())
		copy_list_values(port_number, info.port_number);
	assignCGI(info.cgi_response);
	routes = info.routes;
}

server_info_t	VirtualServer::info() const
{
	server_info_t	info;

	info.host = host;
	info.server_name = server_name;
	info.root_dir = root_dir;
	info.cgi_response = cgi_response;
	info.routes = routes;
	copy_list_values(info.port_number, port_number);
	// info.port_number = port_number;
	return (info);
}


bool	VirtualServer::check_for_cgi(std::string const &file_extention)
{
	if (cgi.find(file_extention) == cgi.end()) {
		return (false);
	} else {
		return (true);
	}
}

/* Executes CGI and Return File descriptor of ready to read pipe */
int	VirtualServer::executeCGI(std::string const &file_extention, std::string const &script, std::string const &query)
{
	std::string fullPath;

	fullPath.append(root_dir);
	fullPath.append(script);
	return ((cgi.find(file_extention))->second.execute(fullPath, query));
}

void	VirtualServer::assignCGI(std::map<std::string, std::string> const &cgi_response)
{
	std::map<std::string, std::string>::const_iterator	cgi_info = cgi_response.begin();

	while (cgi_info != cgi_response.end())
	{
		std::cout << "Setting CGI: " << cgi_info->first << " on " << cgi_info->second << std::endl;
		cgi[cgi_info->first.c_str()] = CGI_Median(cgi_info->second.c_str());
		cgi_info++;
	}
}