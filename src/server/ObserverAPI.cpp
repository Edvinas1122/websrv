#include <ObserverAPI.hpp>

void	ObserverAPI::AssignSocket(const int listening_socket_fd) EXCEPTION
{
	struct epoll_event	ev;

	ev.events = EPOLLIN; //available for read | level triggered
	ev.data.fd = listening_socket_fd;
	if (epoll_ctl(epol_fd, EPOLL_CTL_ADD, listening_socket_fd, &ev) == -1) {
		throw EpollFailure();
	}
}

void	ObserverAPI::AssingClientAgnet(const int client_agent_fd) EXCEPTION
{
	struct epoll_event	ev;

	// ev.events = EPOLLIN | EPOLLET; //edge triggered
	ev.events = EPOLLIN | EPOLLOUT; //available for write
	ev.data.fd = client_agent_fd;
	if (epoll_ctl(epol_fd, EPOLL_CTL_ADD, client_agent_fd, &ev)) {
		throw EpollFailure();
	}
}

void	ObserverAPI::AssingClientAgnet(const int client_agent_fd, uint32_t event_to_observe) EXCEPTION
{
	struct epoll_event	ev;

	// ev.events = EPOLLIN | EPOLLET; //edge triggered
	ev.events = event_to_observe; //available for write
	ev.data.fd = client_agent_fd;
	if (epoll_ctl(epol_fd, EPOLL_CTL_ADD, client_agent_fd, &ev)) {
		throw EpollFailure();
	}
}

void	ObserverAPI::ModClientAgnet(const int client_agent_fd, uint32_t event_to_observe) EXCEPTION
{
	struct epoll_event	ev;

	// ev.events = EPOLLIN | EPOLLET; //edge triggered
	ev.events = event_to_observe; //available for write
	ev.data.fd = client_agent_fd;
	if (epoll_ctl(epol_fd, EPOLL_CTL_MOD, client_agent_fd, &ev)) {
		throw EpollFailure();
	}
}

void	ObserverAPI::RemoveFD(const int fd) EXCEPTION
{
	struct epoll_event	ev;

	ev.events = EPOLLIN; //edge triggered
	ev.data.fd = fd;
	if (epoll_ctl(epol_fd, EPOLL_CTL_DEL, fd, &ev) == 1) {
		throw std::exception();
	}
}

ObserverAPI::ObserverAPI(void)
{
	event_ammount = 15;
	events = NULL;
	epol_fd = epoll_create1(0);
	if (epol_fd == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
}

ObserverAPI::~ObserverAPI(void)
{
	if (events)
		delete[] events;
}

int	ObserverAPI::observe(const int timeout = 0) EXCEPTION
{
	int	count_of_loud_sockets;

	count_of_loud_sockets = epoll_wait(epol_fd, eventArray(), event_ammount, timeout);
	// std::cout << count_of_loud_sockets << std::endl;
	if (count_of_loud_sockets == -1)
		throw EpollFailure();
	return (count_of_loud_sockets);
}

bool	ObserverAPI::checkFD(const int fd)
{
	size_t	i = 0;

	observe(0);
	while (i < event_ammount)
	{
		if (events[i].data.fd == fd)
		{
			// std::cout << "event found" << std::endl;
			if (events[i].events & EPOLLOUT || events[i].events & EPOLLIN)
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (false);
}

bool	ObserverAPI::checkFD(const int fd, const int timeout)
{
	size_t	i = 0;

	observe(timeout);
	while (i < event_ammount)
	{
		if (events[i].data.fd == fd)
		{
			// std::cout << "observer event found" << std::endl;
			if (events[i].events & EPOLLOUT || events[i].events & EPOLLIN)
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (false);
}

bool	ObserverAPI::checkFDforInBound(const int fd)
{
	size_t	i = 0;

	ModClientAgnet(fd, EPOLLIN);
	observe(0);
	ModClientAgnet(fd, EPOLLOUT);
	while (i < event_ammount)
	{
		if (events[i].data.fd == fd)
		{
			// std::cout << "observer event found" << std::endl;
			if (events[i].events & EPOLLOUT || events[i].events & EPOLLIN)
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (false);
}

bool	ObserverAPI::checkNewSockets(void)
{
	observe(0);
	if (events[0].events & EPOLLIN)
		return (true);
	return (false);
}

void	ObserverAPI::setFdAsNotBlocking(const int fd) EXCEPTION
{
	int flags;
	int	result;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		throw std::exception();
	}

	result = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if (result == -1) {
		throw std::exception();
	}
}

ObserverAPI::epollEventArray	ObserverAPI::eventArray(void)
{
	// if (events)
	// 	delete[] events;
	// events = new event[event_ammount];
	// return (events);
	if (!events)
		events = new event[event_ammount];
	memset(events, 0, sizeof(event) * event_ammount);
	return (events);
}