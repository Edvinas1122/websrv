#include <initiator.hpp>
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


/*
	there are equal ammount of { and } symbols
*/
static int	checkEnclosure(std::string const &configurations)
{
	std::size_t index = 0;
	std::size_t brace_count = 0;
	std::size_t close_brace_count = 0;
	
	while (index != std::string::npos)
	{
		index = configurations.find("{", index);
		if (index != std::string::npos) {
			brace_count++;
			index++;
		}
	}
	
	index = configurations.find("}");
	
	while (index != std::string::npos) {
		close_brace_count++;
		index = configurations.find("}", index + 1);
	}
	
	if (brace_count == close_brace_count)
		return (0);
	return (-1);
}

void	ServerInit::basicValidate(std::string const &configurations) EXCEPTION
{
	if (checkEnclosure(configurations))
		throw EnclosureFailure();
}