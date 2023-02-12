#include <VirtualServer.hpp>
#include <Request.hpp>

void	Request::parce(std::string const &received_message, std::string const &port) EXCEPTION
{
	validate_received_message(received_message);
	parceRequestLine(received_message);
	_server = BroadCastExecutor::GetVirtualServer(host, port);
}

broadCastInfo	Request::getBroatCastInfo()
{
	broadCastInfo	info;

	if (_server->check_for_cgi(fileExtension()))
	{
		info.file_fd = _server->executeCGI(fileExtension(), file(), path);
		info.method = GET_METHOD;
		return (info);
	}
	if (method == GET_METHOD)
		info.file_fd = access_path();	
	info.method = method;
	return (info);
}

static int check_requested_access(std::string path_to_get)
{
	return (access(path_to_get.c_str(), R_OK));
}


/*
	https://developer.mozilla.org/en-US/docs/Web/HTTP/Redirections
	implament 301
*/

int	Request::Validate()
{
	std::string	path;

	path.append(_server->info().root_dir);
	path.append("/");
	std::cout << path << std::endl;
	if (!check_requested_access(path))
		return (OK);
	return (BAD);
}

/*
	Parsing ---
	https://www.ibm.com/docs/en/cics-ts/5.3?topic=protocol-http-requests
	A correctly composed HTTP request contains the following elements:
		1. A request line.
		2. A series of HTTP headers, or header fields.
		3. A message body, if needed.
	Request line
	The request line is the first line in the request message. It consists of at least three items:
		1. A method. The method is a one-word command that tells the server what it should do with the resource. For example, the server could be asked to send the resource to the client.
		2. The path component of the URL for the request. The path identifies the resource on the server.
		3. The HTTP version number, showing the HTTP specification to which the client has tried to make the message comply.
		Example:
			GET /software/htp/cics/index.html HTTP/1.1
*/
void	Request::validate_received_message(std::string const &received_message)
{
	(void) received_message;
}

static std::string	determine_file(std::string const &received_message)
{
	std::string str = received_message;

	std::size_t pos = str.find("?");
	if (pos != std::string::npos) {
		str.erase(pos);
	}
	std::cout << str << std::endl;
	return (str);
}

static std::string	determine_query(std::string const &received_message)
{
	std::string str = received_message;

    std::size_t pos = str.find("?");
    if (pos != std::string::npos) {
        str.substr(pos);
    }
    std::cout << str << std::endl;
	return (str);
}

void	Request::parceRequestLine(std::string const &received_message) EXCEPTION
{
	// std::cout << received_message << std::endl;
	method = determine_method(received_message);
	if (method > 8 || method < 1)
		throw InvalidRequest();
	version = determine_version(received_message);
	path = determine_path(received_message);
	file_name = determine_file(path);
	query = determine_query(path);
	host = get_host(received_message);
}

/* Request Line string extractors */
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

std::string	Request::get_host(std::string const &received_message)
{
	std::string string = received_message;
	size_t start_pos = string.find("Host: ") + strlen("Host: ");
	size_t end_pos = string.find("\n", start_pos);
	std::string host = string.substr(start_pos, end_pos - start_pos);
	size_t colon_pos = host.find(":");

	return (host.substr(0, colon_pos));
}

size_t	Request::determine_method(std::string const &received_message)
{
	std::string						token;

	token = find_in_string(received_message, 0, 1);
	return (request_methods[token]);
}

size_t	Request::determine_version(std::string const &received_message)
{
	std::string						token;

	token = find_in_string(received_message, 0, 3);
	return (versions[token]);
}

std::string	Request::determine_path(std::string const &received_message)
{
	std::string	token;

	token = find_in_string(received_message, 0, 2);
	return (token);
}

/* Access */

int	Request::access_path(void)
{
	std::string	path_to_get;

	path_to_get = _server->info().root_dir;
	if (!path.length()) {
		path_to_get.append("/");
		path_to_get.append(_server->info().index);
	}
	else if (path == "/") {
		path_to_get.append(_server->info().index);
	}
	else
		path_to_get.append(path);
	if (check_requested_access(path_to_get) == 0)
		return (open(path_to_get.c_str(), O_RDONLY));
	else
		return (-1);
}

/* 
	URL parts extractors
*/
std::string	Request::fileExtension() const
{
	std::size_t	dotPos;
	std::string	fileExtension;

	dotPos = path.find_last_of(".");
	if (dotPos != std::string::npos) {
		fileExtension = path.substr(dotPos);
		return (fileExtension);
	} else {
		return ("");
	}
}

std::string	Request::file() const
{
    std::string fileName = path;

    size_t query_start = fileName.find('?');
    if (query_start != std::string::npos) {
        fileName.erase(query_start);
		return (fileName);
    }
	return ("");
}

/*
	Map of HTTP request atributes
*/
Request::InfoMap	Request::request_methods = map_methods();
Request::InfoMap	Request::versions = map_versions();

Request::InfoMap	Request::map_methods()
{
	InfoMap	map;

	map["GET"] = GET_METHOD;
	map["HEAD"] = HEAD_METHOD;
	map["POST"] = POST_METHOD;
	map["DELETE"] = DELETE_METHOD;
	map["TRACE"] = TRACE_METHOD;
	map["CONNECT"] = CONNECT_METHOD;
	map["OPTIONS"] = OPTIONS_METHOD;
	return (map);
}

Request::InfoMap	Request::map_versions()
{
	InfoMap	map;

	map["HTTP/1.1"] = VERSION_1_1;
	map["HTTP/1.0"] = VERSION_1_0;
	return (map);
}
