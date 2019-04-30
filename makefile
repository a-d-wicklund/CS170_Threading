output: basic_main.o threads.o
	gcc -g basic_main.o threads.o -o application

basic_main.o: basic_main.c
	gcc -c -g basic_main.c

threads.o: threads.c
	gcc -c -g threads.c

clean:
	rm *.o