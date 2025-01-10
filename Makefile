########################################################################################################################
#                                                       VARIABLE                                                       #
########################################################################################################################

CM			:=	Connection_Messages/

CO			:=	Channel_Operations/

SRCS		:=	Server.cpp			\
				Client.cpp			\
				Channel.cpp			\
				$(CO)Invite.cpp		\
				$(CO)Join.cpp		\
				$(CO)Kick.cpp		\
				$(CO)Mode.cpp		\
				$(CO)Privmsg.cpp	\
				$(CO)Quit.cpp		\
				$(CO)Topic.cpp		\
				$(CO)Who.cpp		\
				$(CM)Nick.cpp		\
				$(CM)Pass.cpp		\
				$(CM)User.cpp		\


SRCS_D		:=	sources/

OBJS_D		:=	objs/

OBJS		=	$(SRCS:%.cpp=$(OBJS_D)%.o)

HEAD		:=	Server.hpp		\
				Client.hpp		\
				Channel.hpp		\

HEAD_D		:=	includes/

CFLAGS		:=	c++ -Wall -Wextra -Werror -std=c++98 -g3

BIN			:=	ircserv

# NOT WORKING the commands doesn't output the logs
RUN			=	$(shell ./ircserv 8080 pass)

########################################################################################################################
#                                                        RULES                                                         #
########################################################################################################################

all			:	msg_compil $(BIN)
				@echo "Success !"

run			:	all
				echo $(RUN)

msg_compil	:
				@echo "Making..."

$(BIN)		:	$(OBJS_D) $(OBJS)
				@$(CFLAGS) -o $(BIN) $(OBJS)

$(OBJS)		:	$(OBJS_D)%.o: $(SRCS_D)%.cpp $(HEAD_D)/*.hpp Makefile
				@$(CFLAGS) -I$(HEAD_D) -c $< -o $@

$(OBJS_D)	:
				@mkdir -p $(OBJS_D) $(OBJS_D)$(CM) $(OBJS_D)$(CO)
########################################################################################################################
#                                                        COMMANDS                                                      #
########################################################################################################################

clean		:
				@$(RM) -r $(OBJS) $(OBJS_D)

fclean		:	clean
				@$(RM) $(BIN)

re			:	fclean all

.PHONY: all clean fclean re
