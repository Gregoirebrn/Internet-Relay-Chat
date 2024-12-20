//
// Created by grebrune on 12/2/24.
//

#pragma once

# include <iostream>
# include <vector>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
//internal libs
# include "Client.hpp"

//define
#define MAX_CHAN 5

class Server;

typedef struct mod_s {
	std::string		chan_key;
	std::string		topic;
	size_t			max_user;
}	mod_t;

typedef std::map<std::string , std::map<std::string, bool> >::iterator	chan_t;
typedef std::map<std::string, bool>::iterator							user_t;
typedef std::vector<std::string>::iterator								vec_t;

template<typename T, size_t N>
T * end(T (&ra)[N]) {
	return ra + N;
}

class Channel : public Client{
private :
	std::map<std::string, std::map<std::string, bool> >	_channel;
	std::map<std::string, mod_t>						_all_chan;
	Client												*_client;
public :
	//Constructors & destructor
	Channel(Client *instance);
	~Channel();
	//Getters
	bool	get_rights(std::string name, std::string chan, int fd_cli);
	int		get_join_arg(std::string buff, int fd_cli, std::vector<std::string> &channel_v, std::vector<std::string> &key_v);
	//Public methods
	int		Canal_Operators(std::string buff, int fd_cli);
	//Channel Operators
	int		Invite(std::string buff, int fd_cli);
	int		Join(std::string buff, int fd_cli);
	int		Kick(std::string buff, int fd_cli);
	int		Mode(std::string buff, int fd_cli);
	int		Privmsg(std::string buff, int fd_cli);
	int		Quit(std::string buff, int fd_cli);
	int		Topic(std::string buff, int fd_cli);
	int		Who(std::string buff, int fd_cli);
	//Sends
	void	send_chan_msg(std::string channel, std::string msg);
	void	send_msg_to_chan(std::string channel, std::string msg, int to_not_send);
	void	send_rpl_name(std::string channel, int fd_cli);
	void	send_rpl_topic(std::string channel, std::string topic, int fd_cli);
	//Utils
	void	CreateChannel(std::string channel, int fd_cli);
	int		check_max_joined(int fd_cli, std::vector<std::string> channel_v);
	};

