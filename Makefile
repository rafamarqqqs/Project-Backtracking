all: list.o graph.o backtrack.o maps.o main.o
	@gcc -std=c99 -g graph.o maps.o backtrack.o list.o main.o -o main -Wall

main.o:
	@gcc -std=c99 -g -c main.c -o main.o -Wall

backtrack.o:
	@gcc -std=c99 -g -c backtrack.c -Wall

maps.o:
	@gcc -std=c99 -g -c maps.c -Wall

graph.o:
	@gcc -std=c99 -g -c list.c graph.c -Wall

list.o:
	@gcc -std=c99 -g -c list.c -Wall

clean:
	@rm *.o main

run:
	@./main
