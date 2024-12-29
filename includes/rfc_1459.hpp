//
// Created by grebrune on 11/27/24.
//

#pragma once

#define USERLEN 12
#define C_USR "USER"

//pass
#define ERR_NEEDMOREPARAMS(command) (":irc_server 461 " + (std::string(command)) + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED ":irc_server 462 :You may not reregister\r\n"
#define ERR_NOTREGISTRED ":irc_server 808 :You must send PASS before any SERVER command\r\n"
#define ERR_PASSWDMISMATCH ":irc_server 464 :Password incorrect\r\n"
#define RPL_WELCOME(client, nick, prefix) (":irc_server 001 " + (client) + " :Welcome to the IRECTION Network, " + (nick) + " " + (prefix) + "\r\n" )

//nickname
#define ERR_NONICKNAMEGIVEN ":irc_server 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick) (":irc_server 432 " + (nick) + ":Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(nick) (":irc_server 433 " + (nick) + ":Nickname is already in use\r\n")
//for multiple server #define ERR_NICKCOLLISION(nick) ("436 " + (nick) + ":Nickname collision KILL from <user>@<host>\r\n")

//kick
#define ERR_USERNOTINCHANNEL(nick, channel) (":irc_server 441 " + (nick) + " " + (channel) + " :They aren't on that channel\r\n")
#define RPL_PART(nick, channel, client) (":" + (nick)) + " KICK " + (channel) + " " + (client) + " \r\n")
#define RPL_KICK(nick, channel) (":" + (nick) + " PART " + (channel) + " \r\n")
#define RPL_SUCCESKICK(nick, channel) ("KICK " + (nick) + " " + (channel) + " \r\n")

//gone
#define ERR_GONE(nick) ("QUIT :Gone to have lunch " + (nick) + " exiting from the network\r\n")
//mode
//#define RPL_CHANNELMODEIS
//#define ERR_NOSUCHNICK
//#define ERR_KEYSET
//#define RPL_BANLIST
//#define RPL_ENDOFBANLIST
//#define ERR_UNKNOWNMODE
//#define ERR_USERSDONTMATCH
//#define RPL_UMODEIS
//#define ERR_UMODEUNKNOWNFLAG

//invite
#define RPL_INVITING(client, nick, channel) (":irc_server 341 " + (client) + " " + (nick) + " " + (channel) + "\r\n")
#define RPL_INVITED(client, nick, channel) (":" + (client) + " INVITE " + (nick) + " " + (channel) + " " + "\r\n")
#define ERR_NOSUCHCHANNEL(channel) (":irc_server 403 " + (channel) + " :No such channel\r\n")
#define ERR_NOTONCHANNEL(channel) (":irc_server 442 " + (channel) + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, channel) (":irc_server 443 " + (nick) + " " + (channel) + " :is already on channel\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (":irc_server 482 " + (client) + (channel) + " :You're not channel operator\r\n")

//topic
#define RPL_NOTOPIC(client, channel) (":irc_server 331 " + (client) + " " + (channel) + " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic) (":irc_server 332 " + (client) + " " + (channel) + " :" + (topic) + "\r\n")
#define RPL_TOPICWHOTIME(client, channel, nick, setat) (":irc_server 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat) + "\r\n")
#define RPL_CHANGETOPIC(client, channel, topic) (":" + (client) + " TOPIC " + (channel) + " " + (topic) + "\r\n")
#define RPL_CREATIONTOPIC(client, channel, topic) (":" + (client) + " TOPIC " + (channel) + " " + (client) + " " + (topic) + "\r\n")
//join
#define RPL_JOIN(nick, channel) (":" + (nick) + " JOIN " + (channel) + "\r\n")
#define RPL_NAMREPLY(client, channel, prenick) (":irc_server 353 " + (client) + " = " + (channel) + " :" + (prenick) + "\r\n")
#define ERR_TOOMANYCHANNELS(client, channel) (":irc_server 405 " + (client) + " " + (channel) + " :You have joined too many channels\r\n")
#define ERR_BADCHANNELKEY(client, channel) (":irc_server 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)\r\n")
#define RPL_ENDOFNAMES(channel) (":irc_server 366 " + (channel) + " :End of /NAMES list\r\n")


//privmsg
#define ERR_NOSUCHNICK(client , nick) (":irc_server 401 " + (client) + " " + (nick) + ": No such nick\r\n")
