//
// Created by grebrune on 12/2/24.
//

#pragma once

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cstdlib>
//internal libs
# include "Client.hpp"
//define
#define MAX_CHAN 5

class Server;

typedef struct mod_s {
	std::vector<std::string>	invite_list;
	std::string					chan_key;
	std::string					topic;
	std::string					set_of_topic; //for RPL_TOPICWHOTIME
	std::string					time; //for RPL_TOPICWHOTIME
	size_t						in_user;
	size_t						max_user; // WARNING JOIN SHOULD CHECK THIS VALUE - 0 means not limits
	bool						t_bool; // WARNING TOPIC SHOULD CHECK THIS BOOL
	bool						i_bool; // WARNING JOIN SHOULD CHECK THIS BOOL
}	mod_t;

typedef std::map<std::string , std::map<std::string, bool> >::iterator	chan_t;
typedef std::map<std::string, bool>::iterator							user_t;
typedef std::vector<std::string>::iterator								vec_t;
typedef std::map<std::string, mod_t>::iterator							all_chan_t;
typedef std::map<std::string, std::string>::iterator					quiz_t;

template<typename T, size_t N>
T * end(T (&ra)[N]) {
	return ra + N;
}

class Channel {
private :
	std::map<std::string, std::map<std::string, bool> >	_channel;
	std::map<std::string, mod_t>						_all_chan;
	Client												*_client;
	std::map<std::string, std::string>					_quiz;
	std::map<std::string, std::string>					_last_question;
public :
	//Constructors & destructor
	Channel(Client *instance);
	~Channel();
	//Getters
	bool	get_rights(std::string name, std::string chan, int fd_cli);
	int		get_join_arg(std::string buff, int fd_cli, std::vector<std::string> &channel_v, std::vector<std::string> &key_v);
	//Public methods
	bool		Canal_Operators(std::string buff, int fd_cli);
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
	bool	Top_Right(std::string name, std::string channel, int fd_cli);
	void	CreateChannel(std::string channel, int fd_cli);
	int		check_max_joined(int fd_cli, std::vector<std::string> channel_v);
	//Mode flags
	int			mode_i(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_k(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_t(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_o(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_l(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			exec_mode(int fd_cli, bool s, char c, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			exec_loop(int fd_cli, std::vector<std::string> v, all_chan_t itc);
	std::string	GetMode(std::string chan);
	//Bot
	void	RecMessage(const std::string &channel, std::string &msg, int fd);
	void	FoundNextQuestion(const std::string &channel);
	void	SendQuestion(const std::string &channel, const std::string &question);
};

