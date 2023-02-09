#include <HTTP_Server.hpp>
#include <algorithm>

template <typename container>
static void	copy_list_values(container &destination, container &source)
{
	std::copy(source.begin(), source.end(), std::back_inserter(destination));
}

void	HTTP_Server::initiateSockets(std::list<std::string> port_numbers)
{
	std::list<std::string>::iterator	iterator = port_numbers.begin();

	while (iterator != port_numbers.end())
	{
		std::cout << (*iterator).c_str() << std::endl;
		initiateListeningSocket((*iterator).c_str());
		iterator++;
	}
}


bool	HTTP_Server::check_for_cgi(std::string const &file_extention)
{
	if (cgi.find(file_extention) == cgi.end()) {
		return (false);
	} else {
		return (true);
	}
}

/* Executes CGI and Return File descriptor of ready to read pipe */
int	HTTP_Server::executeCGI(std::string const &file_extention, std::string const &script, std::string const &query)
{
	std::string fullPath;

	fullPath.append(root_dir);
	fullPath.append(script);
	return ((cgi.find(file_extention))->second.execute(fullPath, query));
}

void	HTTP_Server::assignCGI(std::map<std::string, std::string> const &cgi_response)
{
	std::map<std::string, std::string>::const_iterator	cgi_info = cgi_response.begin();

	while (cgi_info != cgi_response.end())
	{
		std::cout << "Setting CGI: " << cgi_info->first << " on " << cgi_info->second << std::endl;
		cgi[cgi_info->first.c_str()] = CGI_Median(cgi_info->second.c_str());
		cgi_info++;
	}
}

HTTP_Server::HTTP_Server(server_info_t &info)
{
	host = info.host;
	server_name = info.server_name;
	root_dir = info.root_dir;
	index = info.index;
	if (info.cgi_response.size())
		cgi_response = info.cgi_response;
	if (info.port_number.size())
		copy_list_values(port_number, info.port_number);
	listening_socket_fd.reserve(2);
	initiateSockets(info.port_number);
	assignCGI(info.cgi_response);
	routes = info.routes;
}

server_info_t	HTTP_Server::server_info() const
{
	server_info_t	info;

	info.host = host;
	info.server_name = server_name;
	info.root_dir = root_dir;
	info.cgi_response = cgi_response;
	info.listening_socket_fd = listening_socket_fd.begin();
	info.routes = routes;
	return (info);
}