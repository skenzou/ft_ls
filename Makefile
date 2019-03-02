# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/29 10:58:01 by midrissi          #+#    #+#              #
#    Updated: 2019/02/28 15:51:18 by midrissi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls
SRCS = srcs/ft_ls.c
OBJDIR = ./obj/
OBJ = $(patsubst srcs/%.c,$(OBJDIR)%.o,$(SRCS))
CFLAGS = -Wall -Wextra -Werror -Iincludes
LPATH = ./libft/
LIB = $(addprefix $(LPATH),libft.a)

all: $(NAME)

$(NAME): $(OBJDIR)
	@make -C $(LPATH)
	@gcc $(CFLAGS) -o $(NAME) $(LIB) $(OBJ)


$(OBJDIR): $(SRCS)
	@gcc $(CFLAGS) -c $(SRCS)
	@mkdir -p $(OBJDIR)
	@mv $(notdir $(OBJ)) $(OBJDIR)


clean:
	@rm -rf $(OBJDIR)
	@make fclean -C $(LPATH)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LPATH)

re: fclean all

.PHONY: all clean fclean re
