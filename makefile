shell :  main.o q1.o execute.o inAndOut.o
	gcc -o shell main.o q1.o execute.o inAndOut.o

q1.o : q1.c q1.h
	gcc -c q1.c

execute.o : execute.c q1.h
	gcc -c execute.c

inAndOut.o : inAndOut.c q1.h
	gcc -c inAndOut.c

main.o : main.c q1.h
	gcc -c main.c

clean:
	rm shell q1.o main.o execute.o inAndOut.o

