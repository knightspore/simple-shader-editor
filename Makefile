build: 
	gcc main.c \
		-std=c2x \
		-Wall \
		-Wextra \
		-Werror \
		-lraylib \
		-g \
		-o shaderedit

build-strict:
	gcc main.c \
		-std=c2x \
		-fsanitize=address \
		-Wall \
		-Wextra \
		-Werror \
		-pedantic \
		-lraylib \
		-g \
		-o shaderedit

run: 
	./shaderedit

clean: 
	rm ./shaderedit

init:
	rm -f raylib.h raymath.h
	wget https://raw.githubusercontent.com/raysan5/raylib/refs/heads/master/src/raylib.h
	wget https://raw.githubusercontent.com/raysan5/raylib/refs/heads/master/src/raymath.h

all: clean build run
