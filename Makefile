build: 
	gcc main.c \
		-std=c2x \
		-fsanitize=address \
		-Wall \
		-Wextra \
		-Werror \
		-pedantic \
		-lraylib \
		-o shaderedit

run: 
	./shaderedit

clean: 
	rm ./shaderedit

init:
	rm raylib.h raymath.h
	wget https://raw.githubusercontent.com/raysan5/raylib/refs/heads/master/src/raylib.h
	wget https://raw.githubusercontent.com/raysan5/raylib/refs/heads/master/src/raymath.h

all: build run
