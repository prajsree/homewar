

all: main libuser1 libuser2

main: main_demon.c libuser1 libuser2
	gcc -o main main_demon.c -L. -l user1 -l user2

libuser1: user1.c
	gcc -o libuser1.so -fPIC -shared user1.c

libuser2: user2.c
	gcc -o libuser2.so -fPIC -shared user2.c

clean:
	rm -rf main libuser1.so libuser2.so
    
