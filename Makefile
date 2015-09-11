all: list.o graph.o main.o
	@gcc -std=c99 -g graph.o list.o main.o -o main -Wall

main.o:
	@gcc -std=c99 -g -c main.c -o main.o -Wall

graph.o:
	@gcc -std=c99 -g -c list.c graph.c -Wall

list.o:
	@gcc -std=c99 -g -c list.c -Wall

clean:
	@rm *.o main

run:
	@./main
