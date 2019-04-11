CC = arm-linux-gcc
CFLAGS = -Wall -g
SRCS = my_sort.c
OBJS = $(SRCS:.c=.o)
TARGET = my_sort
all: my_sort

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) -c  $(SRCS)

clean:
	-rm -f $(TARGET) *.txt *.o
