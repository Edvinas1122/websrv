#ifndef INITIATOR_HPP
# define INITIATOR_HPP

// #include <ServerExecution.hpp>
#include <includes.hpp>
#include <fstream>
#define SUCESS 0
#define FAILED_SOCKET -1
#define LIST_END NULL
#define HAS_ALLOWED 0
#define MATCH 0

typedef	struct Route_s
{
	bool		directory_listing_enabled;
	std::string	response_dir;
	std::string	upload_dir;
	std::string	redirect;
	short		forbit_methods[8];
}	Route;


typedef struct server_info_s {
		std::list<std::string>				port_number;
		std::string							host;
		std::string							index;
		std::string							server_name;
		std::string							root_dir;
		std::map<std::string, std::string>	cgi_response;
		std::map<std::string, Route>		routes;
} server_info_t;

class ConfigParcer
{
	private:
		std::string	configFileContent;
	public:
		ConfigParcer(const char *config_file_path) EXCEPTION;
		~ConfigParcer() {};

		/* Parsers */
		std::list<server_info_t>	getVirtualServersList();
		std::list<std::string>		getAllSocketsList();

	private:
		
		/* Load File In */
		void	accessCheck(const char *config_file_path) EXCEPTION;
		void	loadFile(const char *config_file_path) EXCEPTION;
		void	removeComents();

		/* Validation */
		void	basicValidate() EXCEPTION;

		/* Extract server definitions from configfileContent */
		template <typename container>
		void		extractVSDefinitionsList(container &servers_definitions);
		std::string	extractVSDefinition(std::string const &content,
						const char *definition, const int occurance) EXCEPTION;
		
		/* Parce methods in parce servers.cpp */
		std::list<server_info_t>	parseServers(std::list<std::string> const &server_definitions);
		server_info_t				parceServer(std::string const &server_info);
		
		/* Atribure parcers */
		template <typename NoKeyExcept, typename ValueExcept>
		std::list<std::string>	get_list_of_values_by_word(std::string const &server_info_text,
									const char *key_word, bool (*ValueTest)(std::string const&)) EXCEPTION;

		template <typename NoKeyExcept, typename ValueExcept>
		std::string	get_value_by_keyword(std::string const &server_info_text,
					const char *key_word, const int occurance, bool (*ValueTest)(std::string const&)) EXCEPTION;

		std::map<std::string, std::string>	parce_cgi(std::string const &server_info) EXCEPTION;
		std::map<std::string, Route>		routeExtract(std::string const &server_info) EXCEPTION;
		std::string							aquire_route(std::string const &server_info_text, const int index) EXCEPTION;
		std::pair<std::string, std::string> get_route_definition(std::string const &content, const int occurance) EXCEPTION;
		Route								parce_route_definition(std::string const &route_definition_text) EXCEPTION;

		/* Atribute Validators */
		static bool	validIndexFile(std::string const &str);
		static bool	isPositiveNumber(std::string const &str);
		static bool	pathcheck(std::string const &path);
		static bool	validServerName(std::string const &str);
		static bool	validCGI(std::string const &str);
		static bool	validPath(std::string const &str);
		static bool	validEnable(std::string const &str);

		/* utils */
		static int	check_match_word(std::string const &content, std::size_t index,
								std::size_t word_len);
		static int	check_allowed_neighbours(char c);
		static std::size_t	find_word(std::string const &content, const char *word_to_match,
								const std::size_t wanted_occurance);
		static std::size_t	find_braket_end(std::string const &content, const std::size_t start_at);

		static std::string	aquire_value(std::string const &server_info_text,
									const int key_len, const int index) EXCEPTION;

	class	ConfFileAccessFailure: public std::exception {};
	class	ValidationFailure: public std::exception {};
		class	EnclosureFailure: public ValidationFailure {};
		class	ParceFailure: public ValidationFailure {};
		class	SemiCollonFailure: public ValidationFailure {};
		class	NoPort: public ValidationFailure {};
		class	InvalidPort: public ValidationFailure {};
			class	NotNumber: public InvalidPort {};
		class	InvalidRoot: public ValidationFailure {};
		class	NoRoot: public ValidationFailure {};
		class	NoHost: public ValidationFailure {};
		class	InvalidHost: public ValidationFailure {};
	class	ValidationException: public std::exception {};
		class	NoServerName: public ValidationException {};
		class	InvalidServerName: public ValidationException {};
		class	NoIndex: public ValidationException {};
		class	WrongIndex: public ValidationException {};
		class	NoCGI: public ValidationException {};
		class	WrongCGIDescriptionAvailable: public ValidationException {};
		class	NoLinks: public ValidationException {};
		class	WrongDirLinks: public ValidationException {};
		class	NoDLInfo: public ValidationException {};
		class	InvalidDLInfo: public ValidationException {};
		class	NoUploadDir: public ValidationException {};
		class	InvalidUploadDir: public ValidationException {};
		class	NoDefaultFile: public ValidationException {};
		class	InvalidDefaultFile: public ValidationException {};
	public:

	class	NoServerAtIndex: public std::exception {};
	
};

#endif