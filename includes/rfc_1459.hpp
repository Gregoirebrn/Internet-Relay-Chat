//
// Created by grebrune on 11/27/24.
//

#pragma once

//pass
#define ERR_NEEDMOREPARAMS(command) ("461 : " + (command) + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED "462 :You may not reregister\r\n"
#define ERR_NOTREGISTRED "808 :You must send PASS before any SERVER command\r\n"
#define ERR_PASSWDMISMATCH "464 :Password incorrect\r\n"

//nickname
#define ERR_NONICKNAMEGIVEN "431 :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick) ("432 " + (nick) + ":Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(nick) ("433 " + (nick) + ":Nickname is already in use\r\n")
//for multiple server #define ERR_NICKCOLLISION(nick) ("436 " + (nick) + ":Nickname collision KILL from <user>@<host>\r\n")

//kick
#define ERR_USERNOTINCHANNEL(client, nick, channel) ("441" + (client) + (nick) + (channel) + " :They aren't on that channel\r\n")
#define RPL_SUCCESKICK(nick, channel, comment) ("Command to kick " + (nick) + " from " + (channel) + " \r\n")

//
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
#define RPL_INVITING(client, nick, channel) ("341 " + (client) + " " + (nick) + " " + (channel) + "\r\n")
#define ERR_NOSUCHCHANNEL(channel) ("403 " + (channel) + " :No such channel\r\n")
#define ERR_NOTONCHANNEL(channel) ("442 " + (channel) + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, channel) ("443 " + (nick) + (channel) + " :is already on channel\r\n")
#define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + (client) + (channel) + " :You're not channel operator\r\n")

//topic
#define RPL_NOTOPIC(client, channel) ("331" + (client) + (channel) + " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic) ("332" + (client) + (channel) + " :" + (topic) + "\r\n")
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ("333" + (client) + " " + (channel) + " " + (nick) + " " + (setat) + "\r\n")

//join
#define RPL_NAMREPLY(client, channel, prenick) ("353 " + (client) + " = " + (channel) + " :" + (prenick) + "\r\n")
#define ERR_TOOMANYCHANNELS(client, channel) ("405 " + (client) + " " + (channel) + " :You have joined too many channels\r\n")
#define ERR_BADCHANNELKEY(client, channel) ("475 " + (client) + " " + (channel) + " :Cannot join channel (+k)\r\n")
#define RPL_ENDOFNAMES(client, channel) ("366 " + (client) + (channel) + " :End of /NAMES list\r\n")


//privmsg
#define ERR_NOSUCHNICK(client , nick) ("401 :" + (client) + ":" + (nick) + ": No such nick\r\n")
