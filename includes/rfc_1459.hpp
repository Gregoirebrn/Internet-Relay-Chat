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
//#define ERR_NOSUCHCHANNEL
//#define ERR_BADCHANMASK
//#define ERR_CHANOPRIVSNEEDED
//#define ERR_NOTONCHANNEL

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
//#define ERR_USERONCHANNEL
//#define RPL_INVITING
//#define RPL_AWAY

//topic
//#define RPL_NOTOPIC
//#define RPL_TOPIC