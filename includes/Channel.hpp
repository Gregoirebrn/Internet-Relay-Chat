//
// Created by grebrune on 12/2/24.
//

#pragma once

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
//internal libs
# include "Client.hpp"

class Server;

typedef struct mod_s {
	std::string					chan_key; //channel key, empty if not set
	std::string					topic;
	std::string					set_of_topic; //for RPL_TOPICWHOTIME
	std::string					time; //for RPL_TOPICWHOTIME
	size_t						in_user;
	size_t						max_user; // WARNING JOIN SHOULD CHECK THIS VALUE - 0 means not limits
	bool						t_bool; // WARNING TOPIC SHOULD CHECK THIS BOOL
	bool						i_bool; // WARNING JOIN SHOULD CHECK THIS BOOL
	std::vector<std::string>	invite_list;
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
	~Channel() {};
	//Getters
	bool	CheckRights(const std::string& name, const std::string& chan, int fd_cli);
	void	JoinArgs(const std::string& buff, std::vector<std::string> &channel_v, std::vector<std::string> &key_v);
	bool	Top_Right(const std::string& name, const std::string& channel, int fd_cli);
	int		CheckModeChannel(vec_t key_it, vec_t chan_it, int fd_cli);
	//Public methods
	void	CreateChannel(std::string channel, int fd_cli);
	bool	Canal_Operators(const std::string& buff, int fd_cli);
	void	Unknown(const std::string& buff, int fd_cli);
	//Channel Operators
	int		Invite(const std::string& buff, int fd_cli);
	int		Join(const std::string& buff, int fd_cli);
	int		Kick(const std::string& buff, int fd_cli);
	int		Mode(const std::string& buff, int fd_cli);
	int		Privmsg(const std::string& buff, int fd_cli);
	int		Quit(const std::string& buff, int fd_cli);
	int		Topic(const std::string& buff, int fd_cli);
	int		Who(const std::string& buff, int fd_cli);
	//Sends
	void	send_chan_msg(const std::string& channel, const std::string& msg);
	void	send_msg_to_chan(const std::string& channel, const std::string& msg, int to_not_send);
	void	send_rpl_name(const std::string& channel, int fd_cli);
	void	send_rpl_topic(const std::string& channel, const std::string& topic, int fd_cli);
	//Utils
	//Mode flags
	int			mode_i(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_k(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_t(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_o(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			mode_l(int fd_cli, bool s, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			exec_mode(int fd_cli, bool s, char c, std::vector<std::string> v, size_t *j, all_chan_t itc);
	int			exec_loop(int fd_cli, std::vector<std::string> v, all_chan_t itc);
	std::string	GetMode(const std::string& chan);
	//Bot
	void	RecMessage(const std::string &channel, std::string &msg, int fd);
	void	FoundNextQuestion(const std::string &channel);
	void	SendQuestion(const std::string &channel, const std::string &question);
};

