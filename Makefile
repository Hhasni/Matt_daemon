# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hhasni <hhasni@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/23 05:39:41 by hhasni            #+#    #+#              #
#    Updated: 2016/06/12 17:32:24 by hhasni           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC	= g++
NAME	= Matt_daemon

SRC	= mattdaemon.cpp \
	  tintinreporter.cpp \
	  user.cpp

OBJ	= $(SRC:.cpp=.o)

CFLAGS	= -I . -I -std=c++1y

RM	= rm -f

all:		$(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

%.o:		%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:		all clean re fclean
