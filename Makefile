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

HEAD		:=	Server.hpp				\
				Client.hpp				\
				Channel.hpp				\
				LimitedModernRfc.hpp	\

HEAD_D		:=	includes/

CFLAGS		:=	c++ -Wall -Wextra -Werror -std=c++98

BIN			:=	ircserv

########################################################################################################################
#                                                        RULES                                                         #
########################################################################################################################

all			:	msg_compil $(BIN)
				@echo "Makefile: Success !"

msg_compil	:
				@echo "Makefile: Making ..."

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
				@echo "Makefile: Removed objs directory."

fclean		:	clean
				@$(RM) $(BIN)
				@echo "Makefile: Removed binary."

re			:	fclean all

.PHONY: all clean fclean re
