CC=gcc
CFLAGS=-O2 -Wall
OBJ=main.o

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm main *.o
