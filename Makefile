#OBJECTS & STRUCTURE#
INITIATOR_FILES = config_file_parcer.cpp basic_validation.cpp parce_servers.cpp
INITIATOR = $(addprefix initiator/,$(INITIATOR_FILES))

# HTTP_METHODS_FILES = BroadCast.cpp GetMethod.cpp ErrorMessage.cpp
# HTTP_METHODS = $(addprefix http_methods/,$(HTTP_METHODS_FILES))

SERVER_FILES = HTTP_Request.cpp HTTP_Request.hpp HTTP_Server.cpp HTTP_Server.hpp \
				HTTP_Server_initiateListeningSocket.cpp ServerExecution.hpp Server_MainLoop.cpp\
				Server_QueHandler.cpp Server_RequestHandle.cpp $(HTTP_METHODS) ObserverAPI.cpp BroadCastExecutor.cpp
SERVER = $(addprefix server/,$(SERVER_FILES))


SRC_FILES = main.cpp $(INITIATOR) $(SERVER)
SRC = $(addprefix src/,$(SRC_FILES))


HEADER_FILE_DIRS_FLAG = -I/includes -Isrc/ -Isrc/initiator -Isrc/server -Isrc/server/http_methods
CPP = -DCPP98=98
FLAGS = -Wall -Wextra -Werror -std=c++98 -g $(HEADER_FILE_DIRS_FLAG) $(CPP) -fsanitize=address -fsanitize=undefined

NAME = server
CC = c++

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean:
	rm $(NAME)

re: clean all