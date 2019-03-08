all:;
	gcc -o serverA.o serverA.c    -lnsl -lresolv
	gcc -o serverB.o serverB.c    -lnsl -lresolv
	gcc -o serverC.o serverC.c    -lnsl -lresolv
	gcc -o aws.o aws.c            -lnsl -lresolv
	gcc -o client client.c        -lnsl -lresolv
serverA:;
	./serverA.o
serverB:; 
	./serverB.o
serverC:;
	./serverC.o
AWS:;
	./aws.o
clean:; 
	rm serverA.o serverB.o serverC.o aws.o client
