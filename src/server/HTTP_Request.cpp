#include <HTTP_Request.hpp>

static void	map_methods(std::map<std::string, size_t> &request_methods)
{
	request_methods["GET"] = GET_METHOD;
	request_methods["HEAD"] = HEAD_METHOD;
	request_methods["POST"] = POST_METHOD;
	request_methods["DELETE"] = DELETE_METHOD;
	request_methods["TRACE"] = TRACE_METHOD;
	request_methods["CONNECT"] = CONNECT_METHOD;
	request_methods["OPTIONS"] = OPTIONS_METHOD;
}

static void	map_versions(std::map<std::string, size_t> &versions)
{
	versions["HTTP/1.1"] = VERSION_1_1;
	versions["HTTP/1.0"] = VERSION_1_0;
}

static std::string	find_in_string(std::string const &received_message, const int column, const int word)
{
	std::string			token;
	int					iterator;
	std::stringstream	line;

	token = received_message;
	iterator = 0;
	while (column > iterator++)
		token = token.substr(token.find('\n', 0), token.length());
	token = token.substr(0, token.find('\n', 0));
	line << token;
	iterator = 0;
	while (word > iterator++)
		line >> token;
	return (token);
}

size_t	HTTP_Request::determine_method(std::string const &received_message)
{
	std::map<std::string, size_t>	request_methods;
	std::string						token;

	map_methods(request_methods);
	token = find_in_string(received_message, 0, 1);
	return (request_methods[token]);
}

size_t	HTTP_Request::determine_version(std::string const &received_message)
{
	std::map<std::string, size_t>	versions;
	std::string						token;

	map_versions(versions);
	token = find_in_string(received_message, 0, 3);
	return (versions[token]);
}

std::string	HTTP_Request::determine_query(std::string const &received_message)
{
	std::string	token;

	token = find_in_string(received_message, 0, 2);
	return (token);
}

void	HTTP_Request::find_method_version_host_query(std::string const &received_message) EXCEPTION
{
	method = determine_method(received_message);
	if (method > 8 || method < 1)
		throw InvalidRequest();
	version = determine_version(received_message);
	// std::cout << this->version << std::endl;
	this->query = determine_query(received_message);
	// std::cout << this->query << std::endl;
}

void	HTTP_Request::parce(std::string const &received_message)
{
	validate_received_message(received_message);
	find_method_version_host_query(received_message);
}

void	HTTP_Request::validate_received_message(std::string const &received_message)
{
	(void) received_message;
}


static int check_requested_access(std::string path_to_get)
{
	return (access(path_to_get.c_str(), R_OK));
}

int	HTTP_Request::access_path(void)
{
	std::string	path_to_get;

	path_to_get = _server->server_info().root_dir;
	if (!query.length()) {
		path_to_get.append("/");
		path_to_get.append(_server->server_info().index);
	}
	else if (query == "/") {
		path_to_get.append(_server->server_info().index);
	}
	else
		path_to_get.append(query);
	if (check_requested_access(path_to_get) == 0)
		return (open(path_to_get.c_str(), O_RDONLY));
	else
		return (-1);
		
}

// BroadCast	*HTTP_Request::GetStreamMethod(const int user_agent_fd)
// {
// 	BroadCast	*stream;
// 	int				file_fd;

// 	file_fd = access_path();
// 	if (file_fd < 0)
// 		return (NULL);
// 	stream = this->set_method(user_agent_fd, file_fd);
// 	return (stream);
// }

// HttpTaskFuncPtr set_method(const int user_agent_fd, const int file_fd);

std::string	HTTP_Request::fileExtension() const
{
	std::size_t	dotPos;
	std::string	fileExtension;

	dotPos = query.find_last_of(".");
	if (dotPos != std::string::npos) {
		fileExtension = query.substr(dotPos);
		return (fileExtension);
	} else {
		return ("");
	}
}

std::string	HTTP_Request::file() const
{
    std::string fileName = query;

    size_t query_start = fileName.find('?');
    if (query_start != std::string::npos) {
        fileName.erase(query_start);
		return (fileName);
    }
	return ("");
}

broadCastInfo	HTTP_Request::getBroatCastInfo()
{
	broadCastInfo	info;

	std::cout << "check for cgi " << fileExtension() << std::endl;
	if (_server->check_for_cgi(fileExtension()))
	{
		info.file_fd = _server->executeCGI(fileExtension(), file(), query);
		info.method = GET_METHOD;
		info.not_first_packet = true;
		return (info);
	}
	if (method == GET_METHOD)
		info.file_fd = access_path();	
	info.method = method;
	info.not_first_packet = true;
	return (info);
}

int	HTTP_Request::Validate()
{
	std::string	path;

	path.append(_server->server_info().root_dir);
	path.append("/");
	path.append(query);
	if (!check_requested_access(path))
		return (OK);
	return (BAD);
}