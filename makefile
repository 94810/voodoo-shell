CC=gcc
CFLAGS=-O0 -g
LIBS=-lreadline
DEPS=
OBJ=shell.o string_handler.o BrainFuck.o

%.o : %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

shell : $(OBJ)
	$(CC) -o vsh  $^ $(LIBS)

run : shell
	./vsh

clean :
	rm *.o
	rm vsh
