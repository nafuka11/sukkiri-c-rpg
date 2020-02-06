CC			= gcc
CFLAGS		= -I$(INCLUDE_DIR)

NAME		= puzmon

SRC_DIR		= ./srcs
INCLUDE_DIR	= ./includes

SRCS		= $(SRC_DIR)/element.c \
			  $(SRC_DIR)/display.c \
			  $(SRC_DIR)/party.c \
			  $(SRC_DIR)/battle.c \
			  $(SRC_DIR)/dungeon.c \
			  $(SRC_DIR)/puzmon.c
OBJS		= $(SRCS:.c=.o)

all		: $(NAME)

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lm

clean	:
	rm -f $(OBJS) $(NAME)

.PHONY	: all clean
