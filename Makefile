all: list.o queue.o graph.o graph.o set.o heap.o main.o
	@gcc -std=c99 -g graph.o queue.o list.o set.o heap.o main.o -o main -Wall

main.o:
	@gcc -std=c99 -g -c main.c -o main.o -Wall

set.o:
	@gcc -std=c99 -g -c set.c -o set.o -Wall

graph.o:
	@gcc -std=c99 -g -c graph.c -o graph.o -Wall

queue.o:
	@gcc -std=c99 -g -c queue.c -o queue.o -Wall

list.o:
	@gcc -std=c99 -g -c list.c -o list.o -Wall

heap.o:
	@gcc -std=c99 -g -c heap.c -o heap.o -Wall

clean:
	@rm *.o main

run:
	@./main
