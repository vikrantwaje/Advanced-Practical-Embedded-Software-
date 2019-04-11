
all: BBG_pthread

BBG_pthread: BBG_pthread.o
	arm-linux-gcc -g -o BBG_pthread BBG_pthread.o -lpthread -lrt

BBG_pthread.o: BBG_pthread.c
	arm-linux-gcc -c BBG_pthread.c 

clean:
	rm -f BBG_pthread *.o
