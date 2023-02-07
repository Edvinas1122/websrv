#ifndef INITIATOR_HPP
# define INITIATOR_HPP

#include <ServerExecution.hpp>
#include <fstream>
#define SUCESS 0
#define FAILED_SOCKET -1
#define LIST_END NULL

typedef struct server_info_s {
		std::list<std::string>				port_number;
		std::string							host;
		std::string							server_name;
		std::string							root_dir;
		bool								directory_listing_enabled;
		std::string							index;
		std::list<std::string>				response_to_dir;
		std::list<std::string>				cgi_response;
		std::string							upload_dir;
		std::vector<int>::const_iterator	listening_socket_fd; // server can return info struct back
} server_info_t;

class ServerInit
{
	private:
		std::list<server_info_t>	servers_info;
	public:
		ServerInit(const char *config_file_path) EXCEPTION;
		~ServerInit() {};

		server_info_t	&get_server_info(int index);

	private:
		
		void	loadFile(const char *config_file_path, std::string &configurations) EXCEPTION;

		/* Validation */
		void	accessCheck(const char *config_file_path) EXCEPTION;
		void	basicValidate(std::string const &configurations) EXCEPTION;

		/* Parce begin */
		std::string	extract_definition(std::string const &content,
						const char *definition, const int occurance) EXCEPTION;
		template <typename container>
		void	get_server_definitions(std::string const &content,
										container &servers_definitions);
		
		/* Parce methods */
		void	parce_servers(std::list<std::string> const &server_definitions);
		void	parce_server(std::string const &server_info);
		
		/* Atribure parcers */
		template <typename NoKeyExcept, typename ValueExcept>
		std::list<std::string>	get_list_of_values_by_word(std::string const &server_info_text,
									const char *key_word, int (*ValueTest)(std::string const&)) EXCEPTION;

		template <typename NoKeyExcept, typename ValueExcept>
		std::string	get_value_by_keyword(std::string const &server_info_text,
					const char *key_word, const int occurance, int (*ValueTest)(std::string const&)) EXCEPTION;

		/* Atribute Validators */
		static int	validIndexFile(std::string const &str);
		static int	isPositiveNumber(std::string const &str);
		static int	pathcheck(std::string const &path);
		static int	validServerName(std::string const &str);

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
	class	ValidationException: public std::exception {};
		class	NoServerName: public ValidationException {};
		class	InvalidServerName: public ValidationException {};
		class	NoIndex: public ValidationException {};
		class	WrongIndex: public ValidationException {};
};

#endif