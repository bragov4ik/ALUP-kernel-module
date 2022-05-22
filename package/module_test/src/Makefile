CC = gcc

all: main
	

clean:
	rm test_program

main: main.c utils.c tests.c
	$(CC) $^ -pthread -o test_program
