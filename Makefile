# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ojimenez <ojimenez@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 16:07:39 by agrimald          #+#    #+#              #
#    Updated: 2024/10/15 18:30:16 by ojimenez         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CPP = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -I./inc -I./inc/commands -I./src -I./src/commands
RM = rm -rf
CYAN = \033[96m
MAGENTA = \033[35m
RED = \033[91m
END_COLOR = \033[0m

SRC = main.cpp src/Message.cpp src/Client.cpp src/Server.cpp src/Channel.cpp \
		src/commands/Pass.cpp src/commands/Quit.cpp src/commands/Nick.cpp \
		src/commands/Join.cpp src/commands/Kick.cpp src/commands/ClasString.cpp \
		src/commands/Privmsg.cpp src/commands/Topic.cpp src/commands/Invite.cpp \
		src/commands/Part.cpp src/commands/Mode.cpp src/commands/Who.cpp
		

INC = inc/Message.hpp inc/Client.hpp inc/Server.hpp inc/Channel.hpp \
		inc/commands/Commad.hpp inc/commands/Join.hpp inc/commands/Kick.hpp \
		inc/commands/Quit.hpp inc/commands/Pass.hpp inc/commands/Nick.hpp \
		inc/commands/ClasString.hpp src/commands/Privmsg.hpp src/commands/Topic.hpp \
		inc/commands/Invite.hpp inc/commands/Part.hpp inc/commands/Mode.hpp inc/commands/Who.hpp
		

OBJECTS = $(SRC:.cpp=.o)

all: banner $(NAME)

#banner:
#	@echo  "$(MAGENTA)\n"
#	@echo  " _____   _____    _____ "
#	@echo  "|_   _| |  __ \  / ____| "
#	@echo  "  | |   | |__) | | | "
#	@echo  "  | |   |  _  /  | | "
#	@echo  "  | |   | | \ \  | |___ "
#	@echo  "|_____| | |  \_\ \_____| "
#	@echo  "\n$(END_COLOR)\n"


banner:
	@echo  "$(MAGENTA)\n"
	@echo  "     ▒▒▄▀▀▀▀▀▄▒▒▒▒▒▄▄▄▄▄▒▒▒     ::::::::::: :::::::::   :::::::: "
	@echo  "     ▒▐░▄░░░▄░▌▒▒▄█▄█▄█▄█▄▒         :+:     :+:    :+: :+:    :+: "
	@echo  "     ▒▐░▀▀░▀▀░▌▒▒▒▒▒░░░▒▒▒▒         +:+     +:+    +:+ +:+        "
	@echo  "     ▒▒▀▄░═░▄▀▒▒▒▒▒▒░░░▒▒▒▒         +#+     +#++:++#:  +#+        "
	@echo  "     ▒▒▐░▀▄▀░▌▒▒▒▒▒▒░░░▒▒▒▒         +#+     +#+    +#+ +#+        "
	@echo  "                                    #+#     #+#    #+# #+#    #+# "
	@echo  "                                ########### ###    ###  ######## "
	@echo "\n$(END_COLOR)"

$(NAME): $(OBJECTS)
		 @echo "$(CYAN)\n𝑻𝒉𝒆 𝒇𝒂𝒃𝒖𝒍𝒐𝒖𝒔 𝑰𝑹𝑪 𝒊𝒔 𝒄𝒐𝒎𝒑𝒊𝒍𝒆𝒅!!!\n$(END_COLOR)"
		 $(CPP) $(CPPFLAGS) -o $@ $^

%.o: %.cpp $(INC) Makefile
		 $(CPP) $(CPPFLAGS) -c $< -o $@

fclean:  banner clean
		 $(RM) $(NAME) $(OBJECTS)

clean:   banner
		 @echo "$(RED)\n𝐼𝑅𝐶 𝐷𝑒𝑙𝑒𝑡𝑒 :𝑐!!!\n$(END_COLOR)"
		 $(RM) $(OBJECTS)

re: clean fclean all

.PHONY: re clean fclean all