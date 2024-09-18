# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrimald <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 16:07:39 by agrimald          #+#    #+#              #
#    Updated: 2024/09/17 16:08:16 by agrimald         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CPP = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -I./inc
RM = rm -rf
CYAN = \033[96m
MAGENTA = \033[35m
RED = \033[91m
END_COLOR = \033[0m

SRC = main.cpp Message.cpp Client.cpp Server.cpp Channel.cpp
INC = inc/Message.hpp inc/Client.hpp inc/Server.hpp inc/Channel.hpp 

OBJECTS = $(SRC:.cpp=.o)

all: banner $(NAME)

banner:
	@echo  "$(MAGENTA)\n"
	@echo  " _____   _____    _____ "
	@echo  "|_   _| |  __ \  / ____| "
	@echo  "  | |   | |__) | | | "
	@echo  "  | |   |  _  /  | | "
	@echo  "  | |   | | \ \  | |___ "
	@echo  "|_____| | |  \_\ \_____| "
	@echo  "\n$(END_COLOR)\n"

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
