CC = gcc
CFLAGS = -Iinc -Wall -Werror -Wextra -pedantic
LDFLAGS = -lSDL2 -lSDL2_image
SRC = src/main.c src/game.c src/3Dproj.c src/poll_events.c src/render.c src/player.c src/color.c 
OBJ = $(SRC:.c=.o)
TARGET = THE_MAZE

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(EXEC) $@ $^ $(LDFLAGS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
