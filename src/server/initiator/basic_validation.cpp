#include <ConfigParcer.hpp>

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

void	ConfigParcer::basicValidate() EXCEPTION
{
	if (checkEnclosure(configFileContent))
		throw EnclosureFailure();
}