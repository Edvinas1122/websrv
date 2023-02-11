#OBJECTS & STRUCTURE#
INITIATOR_FILES = config_file_parcer.cpp basic_validation.cpp parce_servers.cpp
INITIATOR = $(addprefix server/initiator/,$(INITIATOR_FILES))

# HTTP_METHODS_FILES = BroadCast.cpp GetMethod.cpp ErrorMessage.cpp
# HTTP_METHODS = $(addprefix http_methods/,$(HTTP_METHODS_FILES))

SERVER_FILES = Request.cpp Request.hpp VirtualServer.cpp VirtualServer.hpp \
				Server_initiateListeningSocket.cpp ServerExecution.hpp ServerExecutionPrivate.cpp\
				ServerExecutionPublic.cpp $(HTTP_METHODS) ObserverAPI.cpp BroadCastExecutor.cpp\
				CGI.cpp
SERVER = $(addprefix server/,$(SERVER_FILES))


SRC_FILES = main.cpp $(INITIATOR) $(SERVER) terminal.cpp

SRC = $(addprefix src/,$(SRC_FILES))


HEADER_FILE_DIRS_FLAG = -I/includes -Isrc/ -Isrc/server/initiator -Isrc/server -Isrc/server/http_methods
CPP = -DCPP98=98
FLAGS = -Wall -Wextra -Werror -std=c++98 -g $(HEADER_FILE_DIRS_FLAG) $(CPP) -fsanitize=address -fsanitize=undefined

NAME = server
CC = c++

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

terminal: $(SRC)
	$(CC) $(FLAGS) $(SRC) -DTERMINAL=1 -o $(NAME)

clean:
	rm $(NAME)

re: clean all