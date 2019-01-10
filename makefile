#makefile

all: serverA.c serverB.c serverC.c aws.c monitor.c client.c
	gcc -o serverA serverA.c
	gcc -o serverB serverB.c
	gcc -o serverC serverC.c
	gcc -o aws aws.c
	gcc -o monitor monitor.c
	gcc -o client client.c

serverA: serverA.o
	./serverA

serverB: serverB.o
	./serverB

serverC: serverC.o
	./serverC

aws: aws.o
	./aws

monitor: monitor.o
	./monitor

clean: 
	$(RM) serverA
	$(RM) serverB
	$(RM) serverC
	$(RM) aws
	$(RM) monitor
	$(RM) client
	$(RM) serverA.o
	$(RM) serverB.o
	$(RM) serverC.o
	$(RM) aws.o
	$(RM) monitor.o
	$(RM) client.o
