all:
	gcc -Wall main.c typing_functions.c -o app -lraylib -lm -ldl -lpthread -lX11
