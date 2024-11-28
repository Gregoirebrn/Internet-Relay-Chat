//
// Created by grebrune on 11/27/24.
//

#pragma once

//pass
#define ERR_NEEDMOREPARAMS(command) "461 :" command " :Not enough parameters\n"
#define ERR_ALREADYREGISTRED "462 :You may not reregister\n"
#define ERR_NEEDPASS "808 :You must send PASS before any SERVER command\n"

//nickname
//#define ERR_NONICKNAMEGIVEN
//#define ERR_ERRONEUSNICKNAME
//#define ERR_NICKNAMEINUSE
//#define ERR_NICKCOLLISION

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