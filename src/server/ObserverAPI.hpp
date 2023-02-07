#ifndef OBSERVERAPI_HPP
# define OBSERVERAPI_HPP

# include <includes.hpp>
# define MAX_EPOLL_EVENTS 5

// typedef struct event_s: epoll_event
// {
// 	bool	is_socket;
// 	bool	is_noticed;
// } 	event;

typedef struct epoll_event event;

class ObserverAPI
{
	private:
		// typedef std::vector<struct epoll_event*> epollEventArray;
		typedef	event*	epollEventArray;
		// typedef struct epoll_event	event;

	private:
		int					epol_fd;
		// struct epoll_event	events[5];
		epollEventArray		events;
		size_t				event_ammount;

	public:
		ObserverAPI(void);
		~ObserverAPI(void);

		/* Assign Remove Observer */
		void	AssignSocket(const int listening_socket_fd) EXCEPTION;
		void	AssingClientAgnet(const int client_agent_fd) EXCEPTION;
		void	AssingClientAgnet(const int client_agent_fd, uint32_t event_to_observe) EXCEPTION;
		void	ModClientAgnet(const int client_agent_fd, uint32_t event_to_observe) EXCEPTION;

		void	RemoveFD(const int fd) EXCEPTION;

		/* Observe fds set events, return ammount of ready sockets */
		bool	checkFD(const int fd);
		bool	checkFD(const int fd, const int timeout);
		bool	checkFDforInBound(const int fd);
		bool	checkNewSockets(void);

		static void	setFdAsNotBlocking(const int fd) EXCEPTION;
	private:
		epollEventArray	eventArray(void);
		int		observe(const int timeout = 0) EXCEPTION;

	class		EpollFailure: public std::exception {};
};

#endif