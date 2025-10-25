NAME	= prog
SRC		= ${wildcard src/*.c src/**/*.c}
OBJ		= $(SRC:.c=.o)

CC		= cc
INCLUDES := $(shell find include -type d)
CFLAGS = -Wall -Wextra -Werror $(addprefix -I , $(INCLUDES)) -I libs/SDL/include -I libs/SDL_ttf/include -I libs/tinyfiledialogs
LDFLAGS = -L./libs/SDL/build -L./libs/SDL_ttf/build -lSDL3 -lSDL3_ttf 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) libs/tinyfiledialogs/tinyfiledialogs.c -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all