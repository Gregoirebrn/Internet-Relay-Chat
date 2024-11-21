#include "Server.hpp"

void	handler(int sig) {
	(void)sig;
	g_signal = false;
}

int signal_handler() {
	signal(SIGINT, handler);
	return (100);
}