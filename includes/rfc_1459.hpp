//
// Created by grebrune on 11/27/24.
//

#pragma once

#define USERLEN 5
#define C_USR "USER"

//pass
#define ERR_NEEDMOREPARAMS(command) (":irc_server 461 " + (std::string(command)) + " :Not enough parameters\n")
#define ERR_ALREADYREGISTRED ":irc_server 462 :You may not reregister\n"
#define ERR_NOTREGISTRED ":irc_server 808 :You must send PASS before any SERVER command\n"
#define ERR_PWNOTCHECK ":irc_server 808 :You must send PASS before any command\n"
#define ERR_PASSWDMISMATCH ":irc_server 464 :Password incorrect\n"
#define RPL_WELCOME(client, nick) (":irc_server 001 " + (client) + " :Welcome to the Irection Server, " + (nick) + " \n" )

//nickname
#define ERR_NONICKNAMEGIVEN ":irc_server 431 :No nickname given\n"
#define ERR_ERRONEUSNICKNAME(nick) (":irc_server 432 " + (nick) + " :Erroneus nickname\n")
#define ERR_NICKNAMEINUSE(nick) (":irc_server 433 " + (nick) + " :Nickname is already in use\n")
#define RPL_NICK(old, new_nick) (":" + (old) + " NICK " + (new_nick) + "\n")
#define RPL_PRENICK(prefix, new_nick) (":" + (prefix) + " NICK " + (new_nick) + "\n")

//kick
#define ERR_USERNOTINCHANNEL(nick, channel) (":irc_server 441 " + (nick) + " " + (channel) + " :They aren't on that channel\n")
#define RPL_SUCCESKICK(nick, channel, client) (":" + (nick) + " KICK " + (channel) + " " + (client) + "\n")
#define RPL_KICK(prefix, channel, nick) (":" + (prefix) + " KICK " + (channel) + (nick) + "\n")
#define RPL_PART(nick, channel) (":" + (nick) + " PART " + (channel) + "\n")
#define RPL_KICKED(client, channel, kicked) (":" + (client) + " KICK " + (channel) + " " + (kicked) + "\n")
//gone
#define ERR_GONE(nick) ("QUIT :Gone to have lunch " + (nick) + " exiting from the network\n")

//mode
#define RPL_CREAMODE(channel) (":irc_server 324 " + (channel) + " +nrt" + "\r\n")
#define ERR_UNKNOWNMODE(empty) (":irc_server 472 " + (empty) + " " + "+t" + " :is unknown mode char to me\r\n")
//#define RPL_CHANNELMODEIS
//#define ERR_NOSUCHNICK
//#define ERR_KEYSET
//#define RPL_BANLIST
//#define RPL_ENDOFBANLIST

//#define ERR_USERSDONTMATCH
//#define RPL_UMODEIS
//#define ERR_UMODEUNKNOWNFLAG

//invite
#define RPL_INVITING(client, nick, channel) (":irc_server 341 " + (client) + " " + (nick) + " " + (channel) + "\n")
#define RPL_INVITED(client, nick, channel) (":" + (client) + " INVITE " + (nick) + " " + (channel) + " " + "\n")
#define ERR_NOSUCHCHANNEL(channel) (":irc_server 403 " + (channel) + " :No such channel\n")
#define ERR_NOTONCHANNEL(channel) (":irc_server 442 " + (channel) + " :You're not on that channel\n")
#define ERR_USERONCHANNEL(nick, channel) (":irc_server 443 " + (nick) + " " + (channel) + " :is already on channel\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) (":irc_server 482 " + (client) + " " + (channel) + " :You're not channel operator\n")
#define ERR_INVITEONLYCHAN(client, channel) (":irc_server 473 " + (client) + " " + (channel) + " :Cannot join channel (+i)\n")

//topic
#define RPL_NOTOPIC(client, channel) (":irc_server 331 " + (client) + " " + (channel) + " :No topic is set\n")
#define RPL_TOPIC(client, channel, topic) (":irc_server 332 " + (client) + " " + (channel) + " :" + (topic) + "\n")
#define RPL_TOPICWHOTIME(client, channel, nick, setat) (":irc_server 333 " + (client) + " " + (channel) + " " + (nick) + " " + (setat) + "\n")
#define RPL_CHANGETOPIC(client, channel, topic) (":" + (client) + " TOPIC " + (channel) + " " + (topic) + "\n")
#define RPL_CREATIONTOPIC(client, channel, topic) (":" + (client) + " TOPIC " + (channel) + " " + (client) + " " + (topic) + "\n")

//join
#define RPL_JOIN(nick, channel) (":" + (nick) + " JOIN " + (channel) + "\n")
#define RPL_NAMREPLY(client, channel, prenick) (":irc_server 353 " + (client) + " = " + (channel) + " :" + (prenick) + "\n")
#define ERR_TOOMANYCHANNELS(client, channel) (":irc_server 405 " + (client) + " " + (channel) + " :You have joined too many channels\n")
#define ERR_BADCHANNELKEY(client, channel) (":irc_server 475 " + (client) + " " + (channel) + " :Cannot join channel (+k)\n")
#define RPL_ENDOFNAMES(channel) (":irc_server 366 " + (channel) + " :End of /NAMES list\n")
#define ERR_CHANNELISFULL(client, channel) (":irc_server 471 " + (client) + " " + (channel) + " :Cannot join channel (+l)" + "\r\n")
#define ERR_BADCHANMASK(channel) (":irc_server 476 " + (channel) + " :Bad Channel Mask\n")

//privmsg
#define ERR_NOSUCHNICK(client , nick) (":irc_server 401 " + (client) + " " + (nick) + ": No such nick\n")
#define ERR_NODOTMSG (":irc_server bad format for privmsg missing two dots.\n")