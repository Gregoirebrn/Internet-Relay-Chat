//
// Created by grebrune on 12/2/24.
//

#pragma once

# include <iostream>
# include <vector>
#include <bits/stdc++.h>
//internal libs
# include "Client.hpp"
//# include "Server.hpp"

//define
#define MAX_CHAN 5

class Server;

typedef struct mod_s {
	std::string		chan_key;
	std::string		topic;
	size_t			max_user;
	bool			invite;
}	mod_t;

typedef std::map<std::string, std::map<std::string, bool> >::iterator	chan_t;
typedef std::map<std::string, bool>::iterator							user_t;
typedef std::map<std::string, mod_t>::iterator							all_chan_t;


class Channel : public Client{
private :
	std::map<std::string, std::map<std::string, bool> >	_channel;
	std::map<std::string, mod_t>						_all_chan;
public :
	// Constructors & destructor
	Channel();
	~Channel();
	// Public methods
	int		Canal_Operators(std::string buff, int fd_cli);
	int		Kick(std::string buff, int fd_cli);
	int		Invite(std::string buff, int fd_cli);
	int		Topic(std::string buff, int fd_cli);
	int		Mode(std::string buff, int fd_cli);
	int		Join(std::string buff, int fd_cli);
	// Utils for mode
	int 	mode_i(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int 	mode_t(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int 	mode_k(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int 	mode_o(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int 	mode_l(bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int		exec_mode(bool s, char c, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int 	exec_loop(std::vector<std::string> v, all_chan_t itc);
	// Utils for commands
	bool	get_rights(std::string name, std::string chan, int fd_cli);
	int		get_join_arg(std::string buff, int fd_cli, std::vector<std::string> &channel_v, std::vector<std::string> &key_v);
	int		check_max_joined(int fd_cli, std::vector<std::string> channel_v);
	void	send_rpl_name(std::string channel, int fd_cli);
	void	send_rpl_topic(std::string channel, std::string topic, int fd_cli);
	};
